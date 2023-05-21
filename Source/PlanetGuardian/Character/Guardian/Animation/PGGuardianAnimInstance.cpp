// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardianAnimInstance.h"
#include "PGGuardianAnimInstanceProxy.h"
#include "Character/Guardian/PGGuardian.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/PGGameGlobals.h"
#include "ThirdParty/MagicEnum/PGMagicEnum.h"

UPGGuardianAnimInstance::UPGGuardianAnimInstance()
{
}

void UPGGuardianAnimInstance::LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass)
{
	Super::LinkAnimClassLayers(InClass);

	bLinkedLayerChanged = true;
}

void UPGGuardianAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RootYawOffsetMode = EPGRootYawOffsetMode::Accumulate;
}

void UPGGuardianAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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

void UPGGuardianAnimInstance::UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
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

void UPGGuardianAnimInstance::SetupStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	InitialStartDirection = LocalVelocityDirection;
}

void UPGGuardianAnimInstance::UpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto AnimState = ConvertToAnimationState(Node);

	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimState))
	{
		// In start state, we want to keep the direction of the character
		RootYawOffsetMode = EPGRootYawOffsetMode::Hold;
	}
}

void UPGGuardianAnimInstance::UpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto AnimState = ConvertToAnimationState(Node);

	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimState))
	{
		// If stop state blending out, add root yaw offset earlier
		// This is because it enters the idle state when the halted state ends.
		RootYawOffsetMode = EPGRootYawOffsetMode::Accumulate;
	}
}

void UPGGuardianAnimInstance::SetupPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	InitialPivotDirection = LocalVelocityDirection;
	LastPivotTime = Settings.InitialLastPivotTime;
}

void UPGGuardianAnimInstance::UpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	if (LastPivotTime > 0.f)
	{
		LastPivotTime -= Context.GetContext()->GetDeltaTime();
	}
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

void UPGGuardianAnimInstance::ShowDebug()
{
	if (!bShowDebug)
	{
		return;
	}

	const auto& Globals = UPGGameGlobals::Get();

	Globals.PrintMessage(FColor::Blue, TEXT("bIsOnGround: %s"), bIsOnGround ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsCrouching: %s"), bIsCrouching ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsJumping: %s"), bIsJumping ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsFalling: %s"), bIsFalling ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsReloading: %s"), bIsReloading ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsFiring: %s"), bIsFiring ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT("bIsOnADS: %s"), bIsOnADS ? TEXT("true") : TEXT("false"));
	Globals.PrintMessage(FColor::Blue, TEXT(""));

	Globals.PrintMessage(FColor::Red, TEXT("PivotDirection2D: %s"), *PivotDirection2D.ToString());
	Globals.PrintMessage(FColor::Red, TEXT("LastPivotTime: %f"), LastPivotTime);
	Globals.PrintMessage(FColor::Red, TEXT(""));

	Globals.PrintMessage(FColor::Green, TEXT("RootYawOffsetMode: %s"), *FPGMagicEnum::GetEnumString(RootYawOffsetMode));
	Globals.PrintMessage(FColor::Green, TEXT("RootYawOffset: %f"), RootYawOffset);
	Globals.PrintMessage(FColor::Green, TEXT(""));

	Globals.PrintMessage(FColor::White, TEXT("LeanAngle: %f"), LeanAngle);
	Globals.PrintMessage(FColor::White, TEXT("GroundSpeed: %f"), GroundSpeed);
	Globals.PrintMessage(FColor::White, TEXT("LocalVelocityDirectionAngle: %f"), LocalVelocityDirectionAngle);	
	Globals.PrintMessage(FColor::White, TEXT("LocalVelocityDirectionNoOffset: %s"), *FPGMagicEnum::GetEnumString(LocalVelocityDirectionNoOffset));
	Globals.PrintMessage(FColor::White, TEXT("LocalVelocityDirectionAngleWithOffset: %f"), LocalVelocityDirectionAngleWithOffset);
	Globals.PrintMessage(FColor::White, TEXT("LocalVelocityDirection: %s"), *FPGMagicEnum::GetEnumString(LocalVelocityDirection));

	const auto* Guardian = Cast<APGGuardian>(TryGetPawnOwner());
	if (Guardian == nullptr)
	{
		return;
	}

	const auto* Mesh = Cast<USkeletalMeshComponent>(GetOwningComponent());
	if (Mesh == nullptr)
	{
		return;
	}
	
	const auto* CMC = Guardian->GetCharacterMovement();

	const FVector Start = Mesh->GetComponentLocation();
	const FVector End = Start + WorldVelocity.GetSafeNormal() * FVector{ 100.f };

	const FVector VelocityStart = Start + FVector(0.f, 0.f, 1.f);
	const FVector VelocityEnd = FMath::Lerp(Start, End, GroundSpeed / CMC->MaxWalkSpeed);
	
	DrawDebugDirectionalArrow(GetWorld(), VelocityStart, VelocityEnd, 100.f, FColor::Yellow, false, -1.f, SDPG_World, 10.f);
	
}
