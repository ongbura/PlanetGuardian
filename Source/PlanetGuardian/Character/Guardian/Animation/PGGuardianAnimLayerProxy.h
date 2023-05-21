// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "PGGuardianAnimLayerProxy.generated.h"

class UPGGuardianAnimLayer;
class UCharacterMovementComponent;

USTRUCT()
struct FPGGuardianAnimLayerProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UPGGuardianAnimLayer> AnimLayer;
	TWeakObjectPtr<UCharacterMovementComponent> CMC;

public:
	FVector LastUpdateVelocity { 0.f };

	FVector CurrentAcceleration { 0.f };
	
	float BrakingFriction { 0.f };
	
	float GroundFriction { 0.f };
	
	float BrakingFrictionFactor { 0.f };
	
	float BrakingDecelerationWalking { 0.f };
	
	bool bUseSeparateBrakingFriction { false };

	FPGGuardianAnimLayerProxy() = default;

	explicit FPGGuardianAnimLayerProxy(UAnimInstance* InAnimInstance);

protected:
	virtual void Initialize(UAnimInstance* InAnimInstance) override;

	virtual void Uninitialize(UAnimInstance* InAnimInstance) override;
	
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
};
