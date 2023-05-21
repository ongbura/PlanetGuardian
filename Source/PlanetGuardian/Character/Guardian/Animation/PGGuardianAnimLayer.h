// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGuardianAnimLayerSettings.h"
#include "Animation/PGAnimInstance.h"
#include "PGGuardianAnimLayer.generated.h"

class UPGGuardianLocomotionData;
class UPGGuardianAnimInstance;
class UAimOffsetBlendSpace;

USTRUCT(BlueprintType)
struct FPGCardinalDirectionAnimSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right;
};

UCLASS()
class PLANETGUARDIAN_API UPGGuardianAnimLayer : public UPGAnimInstance
{
	GENERATED_BODY()

	friend struct FPGGuardianAnimLayerProxy;

	TWeakObjectPtr<const UPGGuardianAnimInstance> LinkedInstance;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	FPGGuardianAnimLayerSettings Settings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TObjectPtr<UAnimSequence> Idle_ADS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TObjectPtr<UAnimSequence> Idle_HipFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TArray<TObjectPtr<UAnimSequence>> Idle_Breaks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TObjectPtr<UAnimSequence> Crouch_Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TObjectPtr<UAnimSequence> Crouch_Idle_Entry;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Idle")
	TObjectPtr<UAnimSequence> Crouch_Idle_Exit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Starts")
	FPGCardinalDirectionAnimSet Jog_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Starts")
	FPGCardinalDirectionAnimSet ADS_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Starts")
	FPGCardinalDirectionAnimSet Crouch_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Stops")
	FPGCardinalDirectionAnimSet Jog_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Stops")
	FPGCardinalDirectionAnimSet ADS_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Stops")
	FPGCardinalDirectionAnimSet Crouch_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Pivots")
	FPGCardinalDirectionAnimSet Jog_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Pivots")
	FPGCardinalDirectionAnimSet ADS_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Pivots")
	FPGCardinalDirectionAnimSet Crouch_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Left_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Left_180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Right_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Right_180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Crouch_Left_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Crouch_Left_180;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Crouch_Right_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_TurnInPlace")
	TObjectPtr<UAnimSequence> TurnInPlace_Crouch_Right_180;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jog")
	FPGCardinalDirectionAnimSet Jog_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_Apex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_FallLand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_RecoveryAdditive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_StartLoop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Jump")
	TObjectPtr<UAnimSequence> Jump_FallLoop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Walk")
	FPGCardinalDirectionAnimSet Walk_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Walk")
	FPGCardinalDirectionAnimSet Crouch_Walk_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Aiming")
	TObjectPtr<UAnimSequence> Aim_HipFirePose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Aiming")
	TObjectPtr<UAnimSequence> Aim_HipFirePose_Crouch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Aiming")
	TObjectPtr<UAimOffsetBlendSpace> IdleAimOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimSet_Aiming")
	TObjectPtr<UAimOffsetBlendSpace> RelaxedAimOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pivots")
	FVector PivotStartingAcceleration { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pivots")
	float TimeAtPivotStop { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight Data")
	float HipFireUpperBodyOverrideWeight { 0.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight Data")
	float AimOffsetBlendWeight { 1.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Jump")
	float LandRecoveryAlpha { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Jump")
	float TimeFalling { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skel Control Data")
	float HandIK_Right_Alpha { 1.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skel Control Data")
	float HandIK_Left_Alpha { 1.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aiming")
	float HandFKWeight { 1.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	float TurnInPlaceAnimTime { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	float TurnInPlaceRotationDirection { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	float TurnInPlaceRecoveryDirection { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Idle Breaks")
	float IdleBreakDelayTime { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Idle Breaks")
	float TimeUntilNextIdleBreak { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Idle Breaks")
	int32 CurrentIdleBreakIndex { 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Idle Breaks")
	bool bWantsIdleBreak { false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Orientation Warping")
	float OrientationWarpingBlendWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Orientation Warping")
	float OrientationWarpingDegree;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	void UpdateBlendWeightData(float DeltaTime);

	void UpdateJumpFallData(float DeltaTime);

	void UpdateSkeletonControlData();

	void UpdateOrientationWarpingData(float DeltaTime);

	/**
	 * Setup the idle break start time. (float TimeUntilNextIdleBreak).
	 */
	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Reduces the idle break start time by the delta time to play Idle break animations.
	 * When the idle state is blended out, reset the idle break start time.
	 */
	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void LandRecoveryStart(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Determines whether to play the Crouch Entry or Crouch Exit animation when the crouch state changes.
	 */
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupCrouchTransition(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Determines which animation to play based on the current state (Crouch or ADS or neither).
	 */
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateIdleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Determines which idle break animation to play.
	 */
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupIdleBreakAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Determines which animation to play based on the current state (Crouch or ADS or neither).
	 */
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	 * Play the start animation with distance matching.
	 */
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	/**
	* Play the cycle animation with distance matching.
	*/
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateCycleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupPivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdatePivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateHipFireRaiseWeaponPose(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupTurnInPlaceRotationAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="Anim Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateTurnInPlaceRotationAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupTurnInPlaceRotationState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupTurnInPlaceRecoveryState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="State Node", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateTurnInPlaceRecoveryAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool CanPlayIdleBreak();

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe, CompactNodeTitle="LinkedInstance"))
	const UPGGuardianAnimInstance* GetLinkedInstance();

private:
	void ChooseIdleBreakDelayTime();

	void ResetIdleBreakTransitionLogic();

	void ProcessIdleBreakTransitionLogic(float DeltaTime);

	bool ShouldDistanceMatchStop();

	float GetPredictedStopDistance();

	UAnimSequence* SelectTurnInPlaceAnimation(float Direction);

	UAnimSequence* SelectCardinalDirectionAnimationOnStance(const FPGCardinalDirectionAnimSet& Normal,
	                                                        const FPGCardinalDirectionAnimSet& ADS,
	                                                        const FPGCardinalDirectionAnimSet& Crouch) const;

	UAnimSequence* GetDesiredPivotSequence() const;
};
