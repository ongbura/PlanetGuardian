// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PGAnimInstance.h"
#include "PGGuardianAnimationSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "PGGuardianAnimInstance.generated.h"

struct FPGGuardianAnimInstanceProxy;
class UPGGuardianMovementComponent;
struct FAnimNodeReference;
struct FAnimUpdateContext;
class UPGGuardianLocomotionData;

UCLASS()
class PLANETGUARDIAN_API UPGGuardianAnimInstance final : public UPGAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings", meta=(AllowPrivateAccess="true"))
	FPGGuardianAnimSettings Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location", meta=(AllowPrivateAccess="true"))
	FVector WorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location", meta=(AllowPrivateAccess="true"))
	float DeltaDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location", meta=(AllowPrivateAccess="true"))
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation", meta=(AllowPrivateAccess="true"))
	FRotator WorldRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation", meta=(AllowPrivateAccess="true"))
	float DeltaYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation", meta=(AllowPrivateAccess="true"))
	float DeltaYawSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rotation", meta=(AllowPrivateAccess="true"))
	float LeanAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	FVector WorldVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	FVector LocalVelocity2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	float LocalVelocityDirectionAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	float LocalVelocityDirectionAngleWithOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	EPGCardinalDirection LocalVelocityDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	EPGCardinalDirection LocalVelocityDirectionNoOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity", meta=(AllowPrivateAccess="true"))
	bool bHasVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess="true"))
	FVector LocalAcceleration2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess="true"))
	EPGCardinalDirection LocalAccelerationDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess="true"))
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess="true"))
	FVector PivotDirection2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess="true"))
	float LastPivotTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsOnGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bCrouchStateChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float TimeToJumpApex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsRunningIntoWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float DistanceFromGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	float TimeSinceLastFired;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bIsOnADS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bWasOnADS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bADSStateChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Play State", meta=(AllowPrivateAccess="true"))
	bool bIsDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SM Data", meta=(AllowPrivateAccess="true"))
	EPGCardinalDirection InitialPivotDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SM Data", meta=(AllowPrivateAccess="true"))
	EPGCardinalDirection InitialStartDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight", meta=(AllowPrivateAccess="true"))
	float UpperBodyDynamicAdditiveWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Blend Weight", meta=(AllowPrivateAccess="true"))
	bool bIsAnyMontagePlaying;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AimOffset", meta=(AllowPrivateAccess="true"))
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AimOffset", meta=(AllowPrivateAccess="true"))
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place", meta=(AllowPrivateAccess="true"))
	float RootYawOffset;

	FFloatSpringState RootYawOffsetSpringState {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place", meta=(AllowPrivateAccess="true"))
	float TurnYawCurveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place", meta=(AllowPrivateAccess="true"))
	EPGRootYawOffsetMode RootYawOffsetMode;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Linked Layer", meta=(AllowPrivateAccess="true"))
	bool bLinkedLayerChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Orientation Warping", meta=(AllowPrivateAccess="true"))
	float OrientationWarpingBlendWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Orientation Warping", meta=(AllowPrivateAccess="true"))
	float OrientationWarpingDegree;

	bool bIsFirstUpdate;	

public:
	UPGGuardianAnimInstance();

protected:
	virtual void LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass) override;
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsMovingPerpendicularToInitialPivot();

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool ShouldEnableControlRig();

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnUpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnBeginStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnUpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnUpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnBeginPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void OnUpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

private:
	void UpdateLocationData(const FPGGuardianAnimInstanceProxy& Proxy, float DeltaTime);

	void UpdateRotationData(const FPGGuardianAnimInstanceProxy& Proxy, float DeltaTime);

	void UpdateVelocityData(const FPGGuardianAnimInstanceProxy& Proxy);

	void UpdateAccelerationData(const FPGGuardianAnimInstanceProxy& Proxy);

	void UpdateWallDetectionHeuristic();

	void UpdateStateData(const FPGGuardianAnimInstanceProxy& Proxy, float DeltaTime);

	void UpdateBlendWeightData(float DeltaTime);

	void UpdateRootYawOffset(float DeltaTime);

	void UpdateAimingData(const float InPitch);

	void UpdateJumpFallData(const FPGGuardianAnimInstanceProxy& Proxy);

	void UpdateOrientationWarpingData(float DeltaTime);

	void SetRootYawOffset(float InRootYawOffset);

	void ProcessTurnYaw();

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	static EPGCardinalDirection SelectDirectionFromAngle(float Angle, float DeadZone, EPGCardinalDirection CurrentDirection, bool bUseCurrentDirection);

	static EPGCardinalDirection GetOppositeDirection(EPGCardinalDirection Direction);

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
