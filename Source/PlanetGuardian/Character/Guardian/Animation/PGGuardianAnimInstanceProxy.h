// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "PGGuardianAnimInstanceProxy.generated.h"

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

	bool bShouldUpdate { true };
	
	FPGGuardianAnimInstanceProxy() = default;

	explicit FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance);

protected:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
};
