// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardianAnimInstance.h"
#include "PGGuardianAnimInstanceProxy.h"
#include "ThirdParty/MagicEnum/PGMagicEnum.h"

UPGGuardianAnimInstance::UPGGuardianAnimInstance()
{
}

void UPGGuardianAnimInstance::LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass)
{
	Super::LinkAnimClassLayers(InClass);

	bLinkedLayerChanged = true;
}

void UPGGuardianAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	const auto& Proxy = GetProxyOnAnyThread<FPGGuardianAnimInstanceProxy>();

	if (!Proxy.bShouldUpdate)
	{
		return;
	}

	UpdateLocationData(Proxy, DeltaSeconds);
	UpdateRotationData(Proxy, DeltaSeconds);
	UpdateVelocityData(Proxy);
	UpdateAccelerationData(Proxy);
	UpdateWallDetectionHeuristic();
	UpdateStateData(Proxy, DeltaSeconds);
	UpdateBlendWeightData(DeltaSeconds);
	UpdateRootYawOffset(DeltaSeconds);
	UpdateAimingData(Proxy.Pitch);
	UpdateJumpFallData(Proxy);
	UpdateOrientationWarpingData(DeltaSeconds);

	bIsFirstUpdate = false;

	ShowDebug();
}

void UPGGuardianAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();

	bLinkedLayerChanged = false;
}

FAnimInstanceProxy* UPGGuardianAnimInstance::CreateAnimInstanceProxy()
{
	return new FPGGuardianAnimInstanceProxy(this);
}

bool UPGGuardianAnimInstance::IsMovingPerpendicularToInitialPivot()
{
	// We stay in a pivot when pivoting along a line (e.g. triggering a left-right pivot while playing a right-left pivot),
	// but break out if the character makes a perpendicular change in direction.
	const bool bPivotInVertical = InitialPivotDirection == EPGCardinalDirection::Forward ||	InitialPivotDirection == EPGCardinalDirection::Backward;
	const bool bVelocityInVertical = LocalVelocityDirection != EPGCardinalDirection::Forward &&	LocalVelocityDirection != EPGCardinalDirection::Backward;
	const bool bPivotAlongVelocityVertical = bPivotInVertical && bVelocityInVertical;

	const bool bPivotInHorizontal = InitialPivotDirection == EPGCardinalDirection::Left || InitialPivotDirection == EPGCardinalDirection::Right;
	const bool bVelocityInHorizontal = LocalVelocityDirection != EPGCardinalDirection::Left && LocalVelocityDirection != EPGCardinalDirection::Right;
	const bool bPivotAlongVelocityHorizontal = bPivotInHorizontal && bVelocityInHorizontal;

	return bPivotAlongVelocityVertical || bPivotAlongVelocityHorizontal;
}

bool UPGGuardianAnimInstance::ShouldEnableControlRig()
{
	const bool bResult = GetCurveValue(Settings.DisableFootPlacementCurveName) <= 0.f && Settings.bEnableFootPlacement;
	return bResult;
}

void UPGGuardianAnimInstance::OnUpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto AnimState = ConvertToAnimationState(Node);

	if (UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimState))
	{
		TurnYawCurveValue = 0.f;
	}
	else
	{
		ProcessTurnYaw();
	}
}

void UPGGuardianAnimInstance::OnBeginStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	InitialStartDirection = LocalVelocityDirection;
}

void UPGGuardianAnimInstance::OnUpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto AnimState = ConvertToAnimationState(Node);

	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimState))
	{
		// In start state, we want to keep the direction of the character
		RootYawOffsetMode = EPGRootYawOffsetMode::Hold;
	}
}

void UPGGuardianAnimInstance::OnUpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto AnimState = ConvertToAnimationState(Node);

	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimState))
	{
		// If stop state blending out, add root yaw offset earlier
		// This is because it enters the idle state when the halted state ends.
		RootYawOffsetMode = EPGRootYawOffsetMode::Accumulate;
	}
}

void UPGGuardianAnimInstance::OnBeginPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	InitialPivotDirection = LocalVelocityDirection;
}

void UPGGuardianAnimInstance::OnUpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	if (LastPivotTime > 0.f)
	{
		LastPivotTime -= Context.GetContext()->GetDeltaTime();
	}
}

void UPGGuardianAnimInstance::UpdateLocationData(const FPGGuardianAnimInstanceProxy& Proxy, const float DeltaTime)
{
	DeltaDistance = FVector::DistXY(Proxy.WorldLocation, WorldLocation);
	GroundSpeed = FMath::IsNearlyZero(DeltaTime) ? 0.f : DeltaDistance / DeltaTime;

	WorldLocation = Proxy.WorldLocation;

	if (bIsFirstUpdate)
	{
		DeltaDistance = 0.f;
		GroundSpeed = 0.f;
	}
}

void UPGGuardianAnimInstance::UpdateRotationData(const FPGGuardianAnimInstanceProxy& Proxy, const float DeltaTime)
{
	DeltaYaw = Proxy.WorldRotation.Yaw - WorldRotation.Yaw;
	DeltaYawSpeed = FMath::IsNearlyZero(DeltaTime) ? 0.f : DeltaYaw / DeltaTime;

	const float LeanAmountCoefficientOnSlow = Settings.LeanAmountCoefficientOnSlow;
	const float LeanAmountCoefficientOnNormal = Settings.LeanAmountCoefficientOnNormal;

	const float LeanAmountCoefficient = bIsCrouching || bIsOnADS
		                                    ? LeanAmountCoefficientOnSlow
		                                    : LeanAmountCoefficientOnNormal;

	LeanAngle = DeltaYawSpeed * LeanAmountCoefficient;
	LeanAngle = FMath::Clamp(LeanAngle, Settings.MinLeanAmount, Settings.MaxLeanAmount);

	WorldRotation = Proxy.WorldRotation;

	if (bIsFirstUpdate)
	{
		DeltaYaw = 0.f;
		LeanAngle = 0.f;
	}
}

void UPGGuardianAnimInstance::UpdateVelocityData(const FPGGuardianAnimInstanceProxy& Proxy)
{
	const bool bWasMoving = !FMath::IsNearlyZero(LocalVelocity2D.Size());

	WorldVelocity = Proxy.WorldVelocity;
	const FVector WorldVelocity2D { WorldVelocity.X, WorldVelocity.Y, 0.f };

	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	LocalVelocityDirectionAngle = CalculateDirection(WorldVelocity, WorldRotation);

	LocalVelocityDirectionAngleWithOffset = LocalVelocityDirectionAngle - RootYawOffset;

	LocalVelocityDirection = SelectDirectionFromAngle(LocalVelocityDirectionAngleWithOffset,
	                                                  Settings.CardinalDirectionDeadZone, LocalVelocityDirection,
	                                                  bWasMoving);
	LocalVelocityDirectionNoOffset = SelectDirectionFromAngle(LocalVelocityDirectionAngle,
	                                                          Settings.CardinalDirectionDeadZone,
	                                                          LocalVelocityDirectionNoOffset, bWasMoving);

	bHasVelocity = !FMath::IsNearlyZero(LocalVelocity2D.Size2D());
}

void UPGGuardianAnimInstance::UpdateAccelerationData(const FPGGuardianAnimInstanceProxy& Proxy)
{
	const FVector WorldAcceleration2D { Proxy.WorldAcceleration.X, Proxy.WorldAcceleration.Y, 0.f };

	LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	bHasAcceleration = !FMath::IsNearlyZero(UKismetMathLibrary::VSizeXYSquared(LocalAcceleration2D), 0.f);

	// Calculate a cardinal direction from acceleration to be used for pivots.
	// Acceleration communicates player intent better for that purpose than velocity does.
	const auto NewPivotDirection = FMath::Lerp(PivotDirection2D, WorldAcceleration2D.GetSafeNormal(), 0.5f);
	PivotDirection2D = NewPivotDirection.GetSafeNormal();

	const float PivotDirectionAngle = CalculateDirection(PivotDirection2D, WorldRotation);
	const auto PivotDirection = SelectDirectionFromAngle(PivotDirectionAngle, Settings.CardinalDirectionDeadZone,
														 EPGCardinalDirection::Forward, false);
	LocalAccelerationDirection = GetOppositeDirection(PivotDirection);
}

void UPGGuardianAnimInstance::UpdateWallDetectionHeuristic()
{
	// This logic guesses if the character is running into a wall by checking if there's a large angle between velocity and acceleration
	// (i.e. the character is pushing towards the wall but actually sliding to the side) and if the character is trying to accelerate but speed is relatively low.

	const bool bIsAccelerating = UKismetMathLibrary::VSizeXY(LocalAcceleration2D) > 0.1f;
	const bool bIsVelocityLow = UKismetMathLibrary::VSizeXY(LocalVelocity2D) < 200.f;

	const float AccelerationVelocityDot = FVector::DotProduct(LocalAcceleration2D.GetSafeNormal(),
															  LocalVelocity2D.GetSafeNormal());
	const bool bBlockedByWall =
		UKismetMathLibrary::InRange_FloatFloat(AccelerationVelocityDot, -0.6f, 0.6f, true, true);

	bIsRunningIntoWall = bIsAccelerating && bIsVelocityLow && bBlockedByWall;
}

void UPGGuardianAnimInstance::UpdateStateData(const FPGGuardianAnimInstanceProxy& Proxy, const float DeltaTime)
{
	// On ground state
	bIsOnGround = Proxy.bIsOnGround;

	// Crouch state
	const bool bWasCrouchingLastUpdate = bIsCrouching;
	bIsCrouching = Proxy.bIsCrouching;
	bCrouchStateChanged = bWasCrouchingLastUpdate != bIsCrouching;

	bIsOnADS = Proxy.bIsOnADS;
	bIsFiring = Proxy.bIsFiring;
	bIsReloading = Proxy.bIsReloading;
	
	bADSStateChanged = Proxy.bIsOnADS != bWasOnADS;
	bWasOnADS = Proxy.bIsOnADS;

	// Weapon fired state
	TimeSinceLastFired = bIsFiring ? 0.f : TimeSinceLastFired + DeltaTime;

	// in air state
	bIsJumping = Proxy.bIsJumping;
	bIsFalling = Proxy.bIsFalling;
}

void UPGGuardianAnimInstance::UpdateBlendWeightData(const float DeltaTime)
{
	bIsAnyMontagePlaying = IsAnyMontagePlaying();
	const float TempWeight = UKismetMathLibrary::FInterpTo(UpperBodyDynamicAdditiveWeight, 0.f, DeltaTime, 6.f);

	UpperBodyDynamicAdditiveWeight = IsAnyMontagePlaying() && bIsOnGround ? 1.f : TempWeight;
}

void UPGGuardianAnimInstance::UpdateRootYawOffset(float DeltaTime)
{
	// This function handles updating the yaw offset depending on the current state of the Pawn owner.

	if (RootYawOffsetMode == EPGRootYawOffsetMode::Accumulate)
	{
		// When the feet aren't moving (e.g. during Idle),
		// offset the root in the opposite direction to the Pawn owner's rotation to keep the mesh from rotating with the Pawn.
		SetRootYawOffset(RootYawOffset - DeltaYaw);
		return;
	}

	if (RootYawOffsetMode == EPGRootYawOffsetMode::BlendOut)
	{
		// When in motion, smoothly blend out the offset.
		const float BlendedRootYawOffset = UKismetMathLibrary::FloatSpringInterp(
			RootYawOffset, 0.f, RootYawOffsetSpringState, 80.f, 1.f, DeltaTime, 1.f, 0.5f);
		SetRootYawOffset(BlendedRootYawOffset);
	}

	RootYawOffsetMode = EPGRootYawOffsetMode::BlendOut;
}

void UPGGuardianAnimInstance::UpdateAimingData(const float InPitch)
{
	AimPitch = UKismetMathLibrary::NormalizeAxis(InPitch);
}

void UPGGuardianAnimInstance::UpdateJumpFallData(const FPGGuardianAnimInstanceProxy& Proxy)
{
	TimeToJumpApex = bIsJumping ? WorldVelocity.Z * -1.f / Proxy.GravityZ : 0.f;

	DistanceFromGround = Proxy.DistanceFromGround;
}

void UPGGuardianAnimInstance::UpdateOrientationWarpingData(const float DeltaTime)
{
	float SymmetryDegree;	
	if (FMath::Abs(LocalVelocityDirectionAngle) > 90.f)
	{
		if (LocalVelocityDirectionAngle > 0.f)
		{
			SymmetryDegree = FMath::Fmod(LocalVelocityDirectionAngle - 180.f, 180.f);
		}
		else
		{
			SymmetryDegree = FMath::Fmod(LocalVelocityDirectionAngle + 180.f, 180.f);
		}
	}
	else
	{
		SymmetryDegree = LocalVelocityDirectionAngle;
	}

	const float SymmetryDegreeSin = UKismetMathLibrary::DegSin(SymmetryDegree * 2.f);
	const float TargetWeight = FMath::Abs(SymmetryDegreeSin);
	const float TargetDegree = SymmetryDegreeSin * -45.f;

	OrientationWarpingBlendWeight = UKismetMathLibrary::FInterpTo(OrientationWarpingBlendWeight, TargetWeight, DeltaTime, Settings.OrientationWarpingSpeed);
	OrientationWarpingDegree = UKismetMathLibrary::FInterpTo(OrientationWarpingDegree, TargetDegree, DeltaTime, Settings.OrientationWarpingSpeed);
}

void UPGGuardianAnimInstance::SetRootYawOffset(float InRootYawOffset)
{
	if (!Settings.bEnableRootYawOffset)
	{
		RootYawOffset = 0.f;
		AimYaw = 0.f;

		return;
	}

	const float NormalizedRootYawOffset = UKismetMathLibrary::NormalizeAxis(InRootYawOffset);
	const auto& RootYawOffsetClamp = bIsCrouching ? Settings.RootYawOffsetAngleClampCrouched : Settings.RootYawOffsetAngleClamp;

	// We clamp the offset because at large offsets the character has to aim too far backwards, which over twists the spine.
	// The turn in place animations will usually keep up with the offset, but this clamp will cause the feet to slide if the user rotates the camera too quickly.
	// If desired, this clamp could be replaced by having aim animations that can go up to 180 degrees or by triggering turn in place animations more aggressively.
	RootYawOffset = UKismetMathLibrary::ClampAngle(NormalizedRootYawOffset, RootYawOffsetClamp.X, RootYawOffsetClamp.Y);

	// We want aiming to counter the yaw offset to keep the weapon aiming in line with the camera.
	AimYaw = RootYawOffset * -1.f;
}

void UPGGuardianAnimInstance::ProcessTurnYaw()
{
	RootYawOffsetMode = EPGRootYawOffsetMode::Accumulate;
	
	const float PrevTurnYawCurveValue = TurnYawCurveValue;
	const float NowTurnYawCurveValue = GetCurveValue(Settings.RemainingTurnYawCurveName);
	const float TurnYawWeightCurveValue = GetCurveValue(Settings.TurnYawWeightCurveName);
	
	if (FMath::IsNearlyZero(NowTurnYawCurveValue))
	{
		TurnYawCurveValue = 0.f;
		return;
	}
	
	// ???? why / instead of *??
	TurnYawCurveValue = NowTurnYawCurveValue / TurnYawWeightCurveValue;
	
	if (!FMath::IsNearlyZero(PrevTurnYawCurveValue))
	{
		// Turn animation is started.
		
		// Avoid applying the curve delta when the curve first becomes relevant.
		// E.g. When a turn animation starts, the previous curve value will be 0.
		// And the current value will be 90, but no actual rotation has happened yet.
	
		const float ReducedRootYawOffset = RootYawOffset - (TurnYawCurveValue - PrevTurnYawCurveValue);
		SetRootYawOffset(ReducedRootYawOffset);
	}
}

float UPGGuardianAnimInstance::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		if (FVector::DotProduct(RightVector, NormalizedVel) < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

EPGCardinalDirection UPGGuardianAnimInstance::SelectDirectionFromAngle(float Angle, float DeadZone,
                                                                       EPGCardinalDirection CurrentDirection,
                                                                       bool bUseCurrentDirection)
{
	const float AbsAngle = FMath::Abs(Angle);

	// Favor playing Fwd/Bwd anims over strafing anims.
	float FwdDeadZone = DeadZone;
	float BwdDeadZone = DeadZone;

	if (bUseCurrentDirection)
	{
		switch (CurrentDirection)
		{
		case EPGCardinalDirection::Forward:
			// If moving Fwd, double the Fwd dead zone.
			// It should be harder to leave Fwd when moving Fwd.
			// When moving Left/Right, the dead zone will be smaller so we don't rapidly toggle between directions.
			FwdDeadZone *= 2.f;
			break;
		case EPGCardinalDirection::Backward:
			// Same as Fwd.
			BwdDeadZone *= 2.f;
			break;
		default:
			break;
		}
	}

	FwdDeadZone += 45.f;
	BwdDeadZone = 135.f - BwdDeadZone;

	if (AbsAngle <= FwdDeadZone)
	{
		return EPGCardinalDirection::Forward;
	}

	if (AbsAngle >= BwdDeadZone)
	{
		return EPGCardinalDirection::Backward;
	}

	if (Angle > 0.f)
	{
		return EPGCardinalDirection::Right;
	}

	return EPGCardinalDirection::Left;
}

EPGCardinalDirection UPGGuardianAnimInstance::GetOppositeDirection(EPGCardinalDirection Direction)
{
	switch (Direction)
	{
	case EPGCardinalDirection::Forward: return EPGCardinalDirection::Backward;
	case EPGCardinalDirection::Backward: return EPGCardinalDirection::Forward;
	case EPGCardinalDirection::Left: return EPGCardinalDirection::Right;
	case EPGCardinalDirection::Right: return EPGCardinalDirection::Left;
	default:
		checkNoEntry();
		return EPGCardinalDirection::Forward;
	}
}

void UPGGuardianAnimInstance::ShowDebug()
{
	if (!bShowDebug)
	{
		return;
	}
	
	PrintDebugMessage(TEXT("LeanAngle: %f"), LeanAngle);
	PrintDebugMessage(TEXT("GroundSpeed: %f"), GroundSpeed);
	PrintDebugMessage(TEXT("LocalVelocityDirectionAngle: %f"), LocalVelocityDirectionAngle);
	PrintDebugMessage(
		TEXT("LocalVelocityDirectionAngleWithOffset: %f"), LocalVelocityDirectionAngleWithOffset);

	PrintDebugMessage(
		TEXT("LocalVelocityDirection: %s"), *FPGMagicEnum::GetEnumString(LocalVelocityDirection));
	PrintDebugMessage(TEXT(""));
	PrintDebugMessage(
		TEXT("RootYawOffsetMode: %s"), *FPGMagicEnum::GetEnumString(RootYawOffsetMode));
	PrintDebugMessage(TEXT("RootYawOffset: %f"), RootYawOffset);
}
