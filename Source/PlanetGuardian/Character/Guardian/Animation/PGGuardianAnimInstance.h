// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PGAnimInstance.h"
#include "PGGuardianAnimationSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "PGGuardianAnimInstance.generated.h"

struct FPGGuardianAnimInstanceProxy;
struct FAnimNodeReference;
struct FAnimUpdateContext;

UCLASS()
class PLANETGUARDIAN_API UPGGuardianAnimInstance : public UPGAnimInstance
{
	GENERATED_BODY()

	friend class UPGGuardianAnimLayer;
	friend struct FPGGuardianAnimInstanceProxy;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	FPGGuardianAnimInstanceSettings Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location")
	FVector WorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location")
	float DeltaDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation")
	FRotator WorldRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation")
	float DeltaYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation")
	float DeltaYawSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation")
	float LeanAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	FVector WorldVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	FVector LocalVelocity2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	float LocalVelocityDirectionAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	float LocalVelocityDirectionAngleWithOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	EPGCardinalDirection LocalVelocityDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	EPGCardinalDirection LocalVelocityDirectionNoOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	bool bHasVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration")
	FVector LocalAcceleration2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration")
	EPGCardinalDirection LocalAccelerationDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration")
	FVector PivotDirection2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration")
	float LastPivotTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsOnGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bCrouchStateChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float TimeToJumpApex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsRunningIntoWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float DistanceFromGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	float TimeSinceLastFired;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bIsOnADS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bWasOnADS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bADSStateChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State")
	bool bIsDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SM Data")
	EPGCardinalDirection InitialPivotDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SM Data")
	EPGCardinalDirection InitialStartDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight")
	float UpperBodyDynamicAdditiveWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight")
	bool bIsAnyMontagePlaying;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AimOffset")
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AimOffset")
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	float RootYawOffset;

	FFloatSpringState RootYawOffsetSpringState {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	float TurnYawCurveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place")
	EPGRootYawOffsetMode RootYawOffsetMode;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Linked Layer")
	bool bLinkedLayerChanged;

	bool bIsFirstUpdate;	

public:
	UPGGuardianAnimInstance();

protected:
	virtual void LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass) override;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativePostEvaluateAnimation() override;
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsMovingPerpendicularToInitialPivot();

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool ShouldEnableControlRig();

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

private:
	void SetRootYawOffset(float InRootYawOffset);
	
	void ProcessTurnYaw();

	void ShowDebug();
	
	template <typename FmtType, typename... Types>
	void PrintDebugMessage(const FmtType& Fmt, Types&&... Args) const
	{
		if (bShowDebug)
		{
			const FString Message = FString::Printf(Fmt, Forward<Types>(Args)...);
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, Message);
		}
	}
};
