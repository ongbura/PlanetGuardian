// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianAnimInstanceProxy.h"

#include "PGGuardianAnimInstance.h"
#include "Character/Guardian/PGGuardian.h"
#include "PGNativeGameplayTags.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"

FPGGuardianAnimInstanceProxy::FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance)
	: Super(InAnimInstance)
{
}

void FPGGuardianAnimInstanceProxy::Initialize(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::Initialize(InAnimInstance);

	OwnerInstance = CastChecked<UPGGuardianAnimInstance>(InAnimInstance);

	Guardian = Cast<APGGuardian>(OwnerInstance->TryGetPawnOwner());
	if (Guardian.IsValid())
	{
		CMC = Cast<UPGGuardianMovementComponent>(Guardian->GetCharacterMovement());
		check(CMC.IsValid());
	}
}

void FPGGuardianAnimInstanceProxy::Uninitialize(UAnimInstance* InAnimInstance)
{
	CMC.Reset();
	Guardian.Reset();
	OwnerInstance.Reset();
	
	FAnimInstanceProxy::Uninitialize(InAnimInstance);
}

void FPGGuardianAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);

	if (!Guardian.IsValid() || !CMC.IsValid())
	{
		bShouldUpdate = false;
		return;
	}
	
	WorldLocation = Guardian->GetActorLocation();
	WorldRotation = Guardian->GetActorRotation();
	WorldVelocity = Guardian->GetVelocity();
	Pitch = Guardian->GetBaseAimRotation().Pitch;

	WorldAcceleration = CMC->GetCurrentAcceleration();
	bIsOnGround = CMC->IsMovingOnGround();
	bIsCrouching = CMC->IsCrouching();

	if (CMC->MovementMode == MOVE_Falling)
	{
		if (WorldVelocity.Z > 0.f)
		{
			bIsJumping = true;
		}
		else
		{
			bIsFalling = true;
		}
	}
	else
	{
		bIsJumping = false;
		bIsFalling = false;
	}

	GravityZ = CMC->GetGravityZ();
	DistanceFromGround = CMC->GetJumpFallData().DistanceFromGround;

	if (const auto* AbilitySystem = Guardian->GetPGAbilitySystemComponent())
	{
		bIsOnADS = AbilitySystem->HasMatchingGameplayTag(PGGameplayTags::State_Guardian_IsOnADS);
		bIsFiring = AbilitySystem->HasMatchingGameplayTag(PGGameplayTags::State_Guardian_IsFiring);
		bIsReloading = AbilitySystem->HasMatchingGameplayTag(PGGameplayTags::State_Guardian_IsReloading);
	}
	else
	{
		bIsOnADS = false;
		bIsFiring = false;
		bIsReloading = false;
	}

	bShouldUpdate = true;
}

void FPGGuardianAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);

	if (!bShouldUpdate)
	{
		return;
	}

	UpdateLocationData(DeltaSeconds);
	UpdateRotationData(DeltaSeconds);
	UpdateVelocityData();
	UpdateAccelerationData();
	UpdateWallDetectionHeuristic();
	UpdateStateData(DeltaSeconds);
	UpdateBlendWeightData(DeltaSeconds);
	UpdateRootYawOffset(DeltaSeconds);
	UpdateAimingData(Pitch);
	UpdateJumpFallData();

	bIsFirstUpdate = false;
}

void FPGGuardianAnimInstanceProxy::UpdateLocationData(float DeltaTime)
{
	OwnerInstance->DeltaDistance = FVector::DistXY(WorldLocation, OwnerInstance->WorldLocation);
	OwnerInstance->GroundSpeed = FMath::IsNearlyZero(DeltaTime) ? 0.f : OwnerInstance->DeltaDistance / DeltaTime;

	OwnerInstance->WorldLocation = WorldLocation;

	if (bIsFirstUpdate)
	{
		OwnerInstance->DeltaDistance = 0.f;
		OwnerInstance->GroundSpeed = 0.f;
	}
}

void FPGGuardianAnimInstanceProxy::UpdateRotationData(float DeltaTime)
{
	OwnerInstance->DeltaYaw = WorldRotation.Yaw - OwnerInstance->WorldRotation.Yaw;
	OwnerInstance->DeltaYawSpeed = FMath::IsNearlyZero(DeltaTime) ? 0.f : OwnerInstance->DeltaYaw / DeltaTime;

	const float LeanAmountCoefficientOnSlow = OwnerInstance->Settings.LeanAmountCoefficientOnSlow;
	const float LeanAmountCoefficientOnNormal = OwnerInstance->Settings.LeanAmountCoefficientOnNormal;

	const float LeanAmountCoefficient = bIsCrouching || bIsOnADS
											? LeanAmountCoefficientOnSlow
											: LeanAmountCoefficientOnNormal;

	OwnerInstance->LeanAngle = OwnerInstance->DeltaYawSpeed * LeanAmountCoefficient;
	OwnerInstance->LeanAngle = FMath::Clamp(OwnerInstance->LeanAngle, OwnerInstance->Settings.MinLeanAmount, OwnerInstance->Settings.MaxLeanAmount);

	OwnerInstance->WorldRotation = WorldRotation;

	if (bIsFirstUpdate)
	{
		OwnerInstance->DeltaYaw = 0.f;
		OwnerInstance->LeanAngle = 0.f;
	}
}

void FPGGuardianAnimInstanceProxy::UpdateVelocityData()
{
	const bool bWasMoving = !FMath::IsNearlyZero(OwnerInstance->LocalVelocity2D.Size());

	OwnerInstance->WorldVelocity = WorldVelocity;
	const FVector WorldVelocity2D { OwnerInstance->WorldVelocity.X, OwnerInstance->WorldVelocity.Y, 0.f };

	OwnerInstance->LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	OwnerInstance->LocalVelocityDirectionAngle = CalculateDirection(WorldVelocity, WorldRotation);

	OwnerInstance->LocalVelocityDirectionAngleWithOffset = OwnerInstance->LocalVelocityDirectionAngle - OwnerInstance->RootYawOffset;

	OwnerInstance->LocalVelocityDirection = SelectDirectionFromAngle(OwnerInstance->LocalVelocityDirectionAngleWithOffset,
													  OwnerInstance->Settings.CardinalDirectionDeadZone, OwnerInstance->LocalVelocityDirection,
													  bWasMoving);
	OwnerInstance->LocalVelocityDirectionNoOffset = SelectDirectionFromAngle(OwnerInstance->LocalVelocityDirectionAngle,
															  OwnerInstance->Settings.CardinalDirectionDeadZone,
															  OwnerInstance->LocalVelocityDirectionNoOffset, bWasMoving);

	OwnerInstance->bHasVelocity = !FMath::IsNearlyZero(OwnerInstance->LocalVelocity2D.Size2D());
}

void FPGGuardianAnimInstanceProxy::UpdateAccelerationData()
{
	const FVector WorldAcceleration2D { WorldAcceleration.X, WorldAcceleration.Y, 0.f };

	OwnerInstance->LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	OwnerInstance->bHasAcceleration = !FMath::IsNearlyZero(UKismetMathLibrary::VSizeXYSquared(OwnerInstance->LocalAcceleration2D), 0.f);

	// Calculate a cardinal direction from acceleration to be used for pivots.
	// Acceleration communicates player intent better for that purpose than velocity does.
	const auto NewPivotDirection = FMath::Lerp(OwnerInstance->PivotDirection2D, WorldAcceleration2D.GetSafeNormal(), 0.5f);
	OwnerInstance->PivotDirection2D = NewPivotDirection.GetSafeNormal();

	const float PivotDirectionAngle = CalculateDirection(OwnerInstance->PivotDirection2D, WorldRotation);
	const auto PivotDirection = SelectDirectionFromAngle(PivotDirectionAngle, OwnerInstance->Settings.CardinalDirectionDeadZone,
														 EPGCardinalDirection::Forward, false);
	OwnerInstance->LocalAccelerationDirection = GetOppositeDirection(PivotDirection);
}

void FPGGuardianAnimInstanceProxy::UpdateWallDetectionHeuristic()
{
	// This logic guesses if the character is running into a wall by checking if there's a large angle between velocity and acceleration
	// (i.e. the character is pushing towards the wall but actually sliding to the side) and if the character is trying to accelerate but speed is relatively low.

	const bool bIsAccelerating = UKismetMathLibrary::VSizeXY(OwnerInstance->LocalAcceleration2D) > 0.1f;
	const bool bIsVelocityLow = UKismetMathLibrary::VSizeXY(OwnerInstance->LocalVelocity2D) < 200.f;

	const float AccelerationVelocityDot = FVector::DotProduct(OwnerInstance->LocalAcceleration2D.GetSafeNormal(),
															  OwnerInstance->LocalVelocity2D.GetSafeNormal());
	const bool bBlockedByWall =
		UKismetMathLibrary::InRange_FloatFloat(AccelerationVelocityDot, -0.6f, 0.6f, true, true);

	OwnerInstance->bIsRunningIntoWall = bIsAccelerating && bIsVelocityLow && bBlockedByWall;
}

void FPGGuardianAnimInstanceProxy::UpdateStateData(float DeltaTime)
{
	// On ground state
	OwnerInstance->bIsOnGround = bIsOnGround;

	// Crouch state
	OwnerInstance->bCrouchStateChanged = OwnerInstance->bIsCrouching != bIsCrouching;
	OwnerInstance->bIsCrouching = bIsCrouching;

	OwnerInstance->bIsOnADS = bIsOnADS;
	OwnerInstance->bIsFiring = bIsFiring;
	OwnerInstance->bIsReloading = bIsReloading;
	
	OwnerInstance->bADSStateChanged = bIsOnADS != OwnerInstance->bWasOnADS;
	OwnerInstance->bWasOnADS = bIsOnADS;

	// Weapon fired state
	OwnerInstance->TimeSinceLastFired = bIsFiring ? 0.f : OwnerInstance->TimeSinceLastFired + DeltaTime;

	// in air state
    OwnerInstance->bIsJumping = bIsJumping;
	OwnerInstance->bIsFalling = bIsFalling;
}

void FPGGuardianAnimInstanceProxy::UpdateBlendWeightData(float DeltaTime)
{
	OwnerInstance->bIsAnyMontagePlaying = OwnerInstance->IsAnyMontagePlaying();
	const float TempWeight = UKismetMathLibrary::FInterpTo(OwnerInstance->UpperBodyDynamicAdditiveWeight, 0.f, DeltaTime, 6.f);

	OwnerInstance->UpperBodyDynamicAdditiveWeight = OwnerInstance->IsAnyMontagePlaying() && bIsOnGround ? 1.f : TempWeight;
}

void FPGGuardianAnimInstanceProxy::UpdateRootYawOffset(float DeltaTime)
{
	// This function handles updating the yaw offset depending on the current state of the Pawn owner.

	if (OwnerInstance->RootYawOffsetMode == EPGRootYawOffsetMode::Accumulate)
	{
		// When the feet aren't moving (e.g. during Idle),
		// offset the root in the opposite direction to the Pawn owner's rotation to keep the mesh from rotating with the Pawn.
		OwnerInstance->SetRootYawOffset(OwnerInstance->RootYawOffset - OwnerInstance->DeltaYaw);
		return;
	}

	if (OwnerInstance->RootYawOffsetMode == EPGRootYawOffsetMode::BlendOut)
	{
		// When in motion, smoothly blend out the offset.
		const float BlendedRootYawOffset = UKismetMathLibrary::FloatSpringInterp(
			OwnerInstance->RootYawOffset, 0.f, OwnerInstance->RootYawOffsetSpringState, 80.f, 1.f, DeltaTime, 1.f, 0.5f);
		OwnerInstance->SetRootYawOffset(BlendedRootYawOffset);
	}

	OwnerInstance->RootYawOffsetMode = EPGRootYawOffsetMode::BlendOut;
}

void FPGGuardianAnimInstanceProxy::UpdateAimingData(const float InPitch)
{
	OwnerInstance->AimPitch = UKismetMathLibrary::NormalizeAxis(InPitch);
}

void FPGGuardianAnimInstanceProxy::UpdateJumpFallData()
{
	OwnerInstance->TimeToJumpApex = bIsJumping ? WorldVelocity.Z * -1.f / GravityZ : 0.f;

	OwnerInstance->DistanceFromGround = DistanceFromGround;
}

float FPGGuardianAnimInstanceProxy::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
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

EPGCardinalDirection FPGGuardianAnimInstanceProxy::SelectDirectionFromAngle(const float Angle, const float DeadZone,
	const EPGCardinalDirection CurrentDirection, const bool bUseCurrentDirection)
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

EPGCardinalDirection FPGGuardianAnimInstanceProxy::GetOppositeDirection(const EPGCardinalDirection Direction)
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
