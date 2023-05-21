// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/PGAnimInstance.h"
#include "PGGuardianAnimInstanceProxy.generated.h"

class UPGGuardianMovementComponent;
class APGGuardian;
class UPGGuardianAnimInstance;

USTRUCT()
struct FPGGuardianAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	
	FVector WorldLocation { 0.f };

	FRotator WorldRotation { 0.f };

	FVector WorldVelocity { 0.f };

	FVector WorldAcceleration { 0.f };

	float DistanceFromGround { 0.f };

	float GravityZ { 0.f };

	float Pitch { 0.f };

	bool bIsOnGround { false };

	bool bIsCrouching { false };

	bool bIsJumping { false };

	bool bIsFalling { false };

	bool bIsOnADS { false };

	bool bIsFiring { false };

	bool bIsReloading { false };

	bool bIsDashing { false };

	bool bIsFirstUpdate { true };

	bool bShouldUpdate { false };
	
private:
	TWeakObjectPtr<UPGGuardianAnimInstance> OwnerInstance;
	TWeakObjectPtr<APGGuardian> Guardian;
	TWeakObjectPtr<UPGGuardianMovementComponent> CMC;

public:
	FPGGuardianAnimInstanceProxy() = default;

	explicit FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance);

protected:
	virtual void Initialize(UAnimInstance* InAnimInstance) override;

	virtual void Uninitialize(UAnimInstance* InAnimInstance) override;
	
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;

	virtual void Update(float DeltaSeconds) override;

private:
	void UpdateLocationData(float DeltaTime);

	void UpdateRotationData(float DeltaTime);

	void UpdateVelocityData();

	void UpdateAccelerationData();

	void UpdateWallDetectionHeuristic();

	void UpdateStateData(float DeltaTime);

	void UpdateBlendWeightData(float DeltaTime);

	void UpdateRootYawOffset(float DeltaTime);

	void UpdateAimingData(const float InPitch);

	void UpdateJumpFallData();

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	static EPGCardinalDirection SelectDirectionFromAngle(float Angle, float DeadZone, EPGCardinalDirection CurrentDirection, bool bUseCurrentDirection);

	static EPGCardinalDirection GetOppositeDirection(EPGCardinalDirection Direction);
};
