// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianAnimLayer.h"

#include "AnimCharacterMovementLibrary.h"
#include "AnimDistanceMatchingLibrary.h"
#include "PGGuardianAnimInstance.h"
#include "PGGuardianAnimLayerProxy.h"

void UPGGuardianAnimLayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPGGuardianAnimLayer::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!LinkedInstance.IsValid())
	{
		return;
	}

	UpdateBlendWeightData(DeltaSeconds);
	UpdateJumpFallData(DeltaSeconds);
	UpdateSkeletonControlData();
	UpdateOrientationWarpingData(DeltaSeconds);
}

FAnimInstanceProxy* UPGGuardianAnimLayer::CreateAnimInstanceProxy()
{
	return new FPGGuardianAnimLayerProxy(this);
}

void UPGGuardianAnimLayer::UpdateBlendWeightData(const float DeltaTime)
{
	const bool bIsOnStandingADS = !LinkedInstance->bIsCrouching && LinkedInstance->bIsOnADS && LinkedInstance->bIsOnGround;
	const bool bReadyFireOnCrouching = LinkedInstance->bIsOnGround && Settings.bRaiseWeaponAfterFiringWhenCrouched;

	if (bIsOnStandingADS || bReadyFireOnCrouching)
	{
		HipFireUpperBodyOverrideWeight = 0.f;
		AimOffsetBlendWeight = 1.f;

		return;
	}

	// TODO: Weapon cooldown should be handled by weapon system
	const bool bNotWeaponReady = LinkedInstance->TimeSinceLastFired < Settings.RaiseWeaponAfterFiringDuration;

	// Must maintain heap fire animation when in ADS state.
	// Since there is no heap fire animation for crouching and falling, it overwrites the upper body.
	const bool bShouldMaintainHipFireAnimation = LinkedInstance->bIsOnADS && (LinkedInstance->bIsCrouching ||
		!LinkedInstance->bIsOnGround);

	const bool bCanApplyHipFireOverridePose = GetCurveValue(Settings.ApplyHipFireOverridePoseCurveName) > 0.f;

	if (bNotWeaponReady || bShouldMaintainHipFireAnimation || bCanApplyHipFireOverridePose)
	{
		HipFireUpperBodyOverrideWeight = 1.f;
		AimOffsetBlendWeight = 1.f;

		return;
	}

	HipFireUpperBodyOverrideWeight = UKismetMathLibrary::FInterpTo(HipFireUpperBodyOverrideWeight, 0.f, DeltaTime, 1.f);

	// Use aiming aim offset when we are idle or we have root yaw offset
	// Use relaxed aim offset during regular motion
	const bool bInRegularMotion = !LinkedInstance->bHasAcceleration && FMath::Abs(LinkedInstance->RootYawOffset) < 10.f;

	const float TargetAimOffsetBlendWeight = bInRegularMotion ? HipFireUpperBodyOverrideWeight : 1.f;
	AimOffsetBlendWeight = UKismetMathLibrary::FInterpTo(AimOffsetBlendWeight, TargetAimOffsetBlendWeight, DeltaTime,
	                                                     10.f);
}

void UPGGuardianAnimLayer::UpdateJumpFallData(const float DeltaTime)
{
	if (LinkedInstance->bIsFalling)
	{
		TimeFalling += DeltaTime;
	}
	else
	{
		if (LinkedInstance->bIsJumping)
		{
			TimeFalling = 0.f;
		}
	}
}

void UPGGuardianAnimLayer::UpdateSkeletonControlData()
{
	const float DisableRightHandIKCurveValue = GetCurveValue(Settings.DisableRightHandIKCurveName);
	const float DisableLeftHandIKCurveValue = GetCurveValue(Settings.DisableLeftHandIKCurveName);

	const float HandIKWeight = Settings.bDisableHandIK ? 0.f : 1.f;

	HandIK_Right_Alpha = FMath::Clamp(HandIKWeight - DisableRightHandIKCurveValue, 0.f, 1.f);
	HandIK_Left_Alpha = FMath::Clamp(HandIKWeight - DisableLeftHandIKCurveValue, 0.f, 1.f);
}

void UPGGuardianAnimLayer::UpdateOrientationWarpingData(const float DeltaTime)
{
	float SymmetryDegree;
	const float LocalVelocityDirectionAngle = LinkedInstance->LocalVelocityDirectionAngle;

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

	OrientationWarpingBlendWeight = UKismetMathLibrary::FInterpTo(OrientationWarpingBlendWeight, TargetWeight,
	                                                              DeltaTime, Settings.OrientationWarpingSpeed);
	OrientationWarpingDegree = UKismetMathLibrary::FInterpTo(OrientationWarpingDegree, TargetDegree, DeltaTime,
	                                                         Settings.OrientationWarpingSpeed);
}

void UPGGuardianAnimLayer::SetupIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	ChooseIdleBreakDelayTime();
	ResetIdleBreakTransitionLogic();
}

void UPGGuardianAnimLayer::UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const auto State = ConvertToAnimationState(Node);

	if (!IsStateBlendingOut(Context, State))
	{
		ProcessIdleBreakTransitionLogic(Context.GetContext()->GetDeltaTime());
	}
}

void UPGGuardianAnimLayer::LandRecoveryStart(const FAnimUpdateContext& Context,
                                             const FAnimNodeReference& Node)
{
	LandRecoveryAlpha = UKismetMathLibrary::MapRangeClamped(TimeFalling, 0.f, 0.4f, 0.1f, 1.f);

	if (LinkedInstance->bIsCrouching)
	{
		LandRecoveryAlpha *= 0.5f;
	}
}

void UPGGuardianAnimLayer::SetupCrouchTransition(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	if (LinkedInstance->bIsCrouching)
	{
		SetSequenceAsPlayer(Node, Crouch_Idle_Entry);
	}
	else
	{
		SetSequenceAsPlayer(Node, Crouch_Idle_Exit);
	}
}

void UPGGuardianAnimLayer::UpdateIdleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	UAnimSequence* IdleSequence;
	if (LinkedInstance->bIsCrouching)
	{
		IdleSequence = Crouch_Idle;
	}
	else if (LinkedInstance->bIsOnADS)
	{
		IdleSequence = Idle_ADS;
	}
	else
	{
		IdleSequence = Idle_HipFire;
	}

	SetSequenceWithInertialBlendingAsPlayer(Context, Node, IdleSequence);
}

void UPGGuardianAnimLayer::SetupIdleBreakAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	SetSequenceAsPlayer(Node, Idle_Breaks[CurrentIdleBreakIndex++]);

	if (CurrentIdleBreakIndex >= Idle_Breaks.Num())
	{
		CurrentIdleBreakIndex = 0;
	}
}

void UPGGuardianAnimLayer::SetupStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto* Sequence = SelectCardinalDirectionAnimationOnStance(Jog_Start_Cardinals, ADS_Start_Cardinals,
	                                                          Crouch_Start_Cardinals);

	auto Evaluator = SetSequenceAsEvaluator(Node, Sequence);
	USequenceEvaluatorLibrary::SetExplicitTime(Evaluator, 0.f);
}

void UPGGuardianAnimLayer::UpdateStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto Evaluator = ConvertToSequenceEvaluator(Node);

	UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(Context, Evaluator, LinkedInstance->DeltaDistance,
	                                                            Settings.DistanceCurveName, Settings.PlayRateClamp);
}

void UPGGuardianAnimLayer::UpdateCycleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto* Sequence = SelectCardinalDirectionAnimationOnStance(Jog_Cardinals, Walk_Cardinals, Crouch_Walk_Cardinals);
	auto Player = SetSequenceWithInertialBlendingAsPlayer(Context, Node, Sequence);

	UAnimDistanceMatchingLibrary::SetPlayrateToMatchSpeed(Player, LinkedInstance->GroundSpeed, Settings.PlayRateClamp);
}

void UPGGuardianAnimLayer::SetupStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto* Sequence = SelectCardinalDirectionAnimationOnStance(Jog_Stop_Cardinals, ADS_Stop_Cardinals,
	                                                          Crouch_Stop_Cardinals);
	auto Evaluator = SetSequenceAsEvaluator(Node, Sequence);

	if (ShouldDistanceMatchStop())
	{
		// If we got here, and we can't distance match a stop on start, match to 0 distance
		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(Evaluator, 0.f, Settings.DistanceCurveName);
	}
}

void UPGGuardianAnimLayer::UpdateStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto Evaluator = ConvertToSequenceEvaluator(Node);

	if (!ShouldDistanceMatchStop())
	{
		USequenceEvaluatorLibrary::AdvanceTime(Context, Evaluator);

		return;
	}

	const float DistanceToMatch = GetPredictedStopDistance();
	if (DistanceToMatch > 0.f)
	{
		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(Evaluator, DistanceToMatch, Settings.DistanceCurveName);
	}
	else
	{
		USequenceEvaluatorLibrary::AdvanceTime(Context, Evaluator);
	}
}

void UPGGuardianAnimLayer::SetupPivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	PivotStartingAcceleration = LinkedInstance->LocalAcceleration2D;

	auto* Sequence = GetDesiredPivotSequence();
	auto Evaluator = SetSequenceAsEvaluator(Node, Sequence);

	USequenceEvaluatorLibrary::SetExplicitTime(Evaluator, 0.f);
	TimeAtPivotStop = 0.f;
}

void UPGGuardianAnimLayer::UpdatePivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto Evaluator = ConvertToSequenceEvaluator(Node);

	const float AccumulatedTime = USequenceEvaluatorLibrary::GetAccumulatedTime(Evaluator);

	if (LinkedInstance->LastPivotTime > 0.f)
	{
		// Allow switching the selected pivot for a short duration at the beginning
		auto* NewDesiredSequence = GetDesiredPivotSequence();

		if (NewDesiredSequence != USequenceEvaluatorLibrary::GetSequence(Evaluator))
		{
			USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, Evaluator, NewDesiredSequence);
			PivotStartingAcceleration = LinkedInstance->LocalAcceleration2D;
		}
	}

	// Does acceleration oppose velocity?
	const float Dot = FVector::DotProduct(LinkedInstance->LocalAcceleration2D, LinkedInstance->LocalVelocity2D);

	if (Dot < 0.f)
	{
		const auto& Proxy = GetProxyOnAnyThread<FPGGuardianAnimLayerProxy>();
		const float DistanceToTarget = UAnimCharacterMovementLibrary::PredictGroundMovementPivotLocation(
			Proxy.CurrentAcceleration, Proxy.LastUpdateVelocity, Proxy.GroundFriction).Size2D();

		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(Evaluator, DistanceToTarget, Settings.DistanceCurveName);
		TimeAtPivotStop = AccumulatedTime;

		return;
	}

	UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(Context, Evaluator, LinkedInstance->DeltaDistance,
	                                                            Settings.DistanceCurveName, Settings.PlayRateClamp);
}

void UPGGuardianAnimLayer::UpdateHipFireRaiseWeaponPose(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	auto* Sequence = LinkedInstance->bIsCrouching? Aim_HipFirePose_Crouch.Get() : Aim_HipFirePose.Get();

	SetSequenceAsEvaluator(Node, Sequence);
}

void UPGGuardianAnimLayer::SetupFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto Evaluator = ConvertToSequenceEvaluator(Node);

	USequenceEvaluatorLibrary::SetExplicitTime(Evaluator, 0.f);
}

void UPGGuardianAnimLayer::UpdateFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	auto Evaluator = ConvertToSequenceEvaluator(Node);

	UAnimDistanceMatchingLibrary::DistanceMatchToTarget(Evaluator, LinkedInstance->DistanceFromGround, Settings.JumpDistanceCurveName);
}

void UPGGuardianAnimLayer::SetupTurnInPlaceRotationAnim(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	TurnInPlaceAnimTime = 0.f;

	auto Evaluator = ConvertToSequenceEvaluator(Node);
	USequenceEvaluatorLibrary::SetExplicitTime(Evaluator, 0.f);
}

void UPGGuardianAnimLayer::UpdateTurnInPlaceRotationAnim(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	auto* Sequence = SelectTurnInPlaceAnimation(TurnInPlaceRotationDirection);
	auto Evaluator = SetSequenceWithInertialBlendingAsEvaluator(Context, Node, Sequence);

	TurnInPlaceAnimTime += Context.GetContext()->GetDeltaTime();
	USequenceEvaluatorLibrary::SetExplicitTime(Evaluator, TurnInPlaceAnimTime);
}

void UPGGuardianAnimLayer::SetupTurnInPlaceRotationState(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	TurnInPlaceRotationDirection = LinkedInstance->RootYawOffset * -1.f;
}

void UPGGuardianAnimLayer::SetupTurnInPlaceRecoveryState(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	TurnInPlaceRecoveryDirection = TurnInPlaceRotationDirection;
}

void UPGGuardianAnimLayer::UpdateTurnInPlaceRecoveryAnim(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node)
{
	auto* Sequence = SelectTurnInPlaceAnimation(TurnInPlaceRecoveryDirection);

	SetSequenceWithInertialBlendingAsPlayer(Context, Node, Sequence);
}

bool UPGGuardianAnimLayer::CanPlayIdleBreak()
{
	if (Idle_Breaks.Num() == 0)
	{
		return false;
	}

	if (!LinkedInstance->bIsCrouching && !LinkedInstance->bIsOnADS && !LinkedInstance->bIsFiring && !LinkedInstance->
		bIsAnyMontagePlaying && !LinkedInstance->bHasVelocity && !LinkedInstance->bIsJumping)
	{
		return true;
	}

	return false;
}

const UPGGuardianAnimInstance* UPGGuardianAnimLayer::GetLinkedInstance()
{
	if (LinkedInstance.IsValid())
	{
		return LinkedInstance.Get();
	}

	return nullptr;
}

void UPGGuardianAnimLayer::ChooseIdleBreakDelayTime()
{
	// Use some logic that depends on the location of the Pawn owner to have roughly consistent behavior across clients
	// without having every character play an idle break at the same time.
	const int32 IndexFromLocation = static_cast<int32>(FMath::Abs(
		LinkedInstance->WorldLocation.X + LinkedInstance->WorldLocation.Y));
	IdleBreakDelayTime = IndexFromLocation / 10 + 6;
}

void UPGGuardianAnimLayer::ResetIdleBreakTransitionLogic()
{
	TimeUntilNextIdleBreak = IdleBreakDelayTime;
}

void UPGGuardianAnimLayer::ProcessIdleBreakTransitionLogic(float DeltaTime)
{
	if (CanPlayIdleBreak())
	{
		TimeUntilNextIdleBreak -= DeltaTime;
	}
	else
	{
		ResetIdleBreakTransitionLogic();
	}
}

bool UPGGuardianAnimLayer::ShouldDistanceMatchStop()
{
	return LinkedInstance->bHasVelocity && !LinkedInstance->bHasAcceleration;
}

float UPGGuardianAnimLayer::GetPredictedStopDistance()
{
	const auto* MainAnimBP = GetLinkedInstance();
	const auto& Proxy = GetProxyOnAnyThread<FPGGuardianAnimLayerProxy>();

	const FVector PredictedStopLocation = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
		Proxy.LastUpdateVelocity, Proxy.bUseSeparateBrakingFriction, Proxy.BrakingFriction, Proxy.GroundFriction,
		Proxy.BrakingFrictionFactor, Proxy.BrakingDecelerationWalking);
	return PredictedStopLocation.Size2D();
}

UAnimSequence* UPGGuardianAnimLayer::SelectTurnInPlaceAnimation(float Direction)
{
	if (LinkedInstance->bIsCrouching)
	{
		if (Direction > 0.f)
		{
			if (FMath::Abs(Direction) > 90.f)
			{
				return TurnInPlace_Crouch_Right_180;
			}
			
			return TurnInPlace_Crouch_Right_90;
		}
		
		if (FMath::Abs(Direction) > 90.f)
		{
			return TurnInPlace_Crouch_Left_180;
		}
			
		return TurnInPlace_Crouch_Left_90;
	}

	if (Direction > 0.f)
	{
		if (FMath::Abs(Direction) > 90.f)
		{
			return TurnInPlace_Right_180;
		}

		return TurnInPlace_Right_90;
	}

	if (FMath::Abs(Direction) > 90.f)
	{
		return TurnInPlace_Left_180;
	}

	return TurnInPlace_Left_90;
}

UAnimSequence* UPGGuardianAnimLayer::SelectCardinalDirectionAnimationOnStance(const FPGCardinalDirectionAnimSet& Normal,
                                                                              const FPGCardinalDirectionAnimSet& ADS,
                                                                              const FPGCardinalDirectionAnimSet& Crouch)
const
{
	const FPGCardinalDirectionAnimSet* AnimSet;

	if (LinkedInstance->bIsCrouching)
	{
		AnimSet = &Crouch;
	}
	else if (LinkedInstance->bIsOnADS)
	{
		AnimSet = &ADS;
	}
	else
	{
		AnimSet = &Normal;
	}

	switch (LinkedInstance->LocalVelocityDirection)
	{
	case EPGCardinalDirection::Forward: return AnimSet->Forward;
	case EPGCardinalDirection::Backward: return AnimSet->Backward;
	case EPGCardinalDirection::Left: return AnimSet->Left;
	case EPGCardinalDirection::Right: return AnimSet->Right;
	default:
		checkNoEntry();
	}

	return nullptr;
}

UAnimSequence* UPGGuardianAnimLayer::GetDesiredPivotSequence() const
{
	const FPGCardinalDirectionAnimSet* AnimSet;

	if (LinkedInstance->bIsCrouching)
	{
		AnimSet = &Crouch_Pivot_Cardinals;
	}
	else if (LinkedInstance->bIsOnADS)
	{
		AnimSet = &ADS_Pivot_Cardinals;
	}
	else
	{
		AnimSet = &Jog_Pivot_Cardinals;
	}

	switch (LinkedInstance->LocalAccelerationDirection)
	{
	case EPGCardinalDirection::Forward: return AnimSet->Forward;
	case EPGCardinalDirection::Backward: return AnimSet->Backward;
	case EPGCardinalDirection::Left: return AnimSet->Left;
	case EPGCardinalDirection::Right: return AnimSet->Right;
	default:
		checkNoEntry();
	}

	return nullptr;
}
