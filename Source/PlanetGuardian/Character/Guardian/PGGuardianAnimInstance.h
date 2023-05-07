// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "PGGuardianAnimInstance.generated.h"

class UCharacterMovementComponent;
class UCapsuleComponent;
class UPGCameraShake;

UCLASS()
class UPGGuardianAnimData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float GroundSpeed { 0.f };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsMoving { false };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsAccelerating { false };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsInAir { false };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsCrouching { false };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Direction { 0.f };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FRotator PreviousRotation { 0.f };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float LeanAmount { 0.f };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsHovering { false };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector InteractWorldLocation { 0.f };

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsInactive { false };
};

USTRUCT()
struct FPGGuardianAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	TWeakObjectPtr<UCapsuleComponent> CapsuleComp { nullptr };
	
	TWeakObjectPtr<UCharacterMovementComponent> MovementComp { nullptr };

	TWeakObjectPtr<class UPGGuardianAnimInstance> AnimInstance { nullptr };

	FPGGuardianAnimInstanceProxy() = default;

	explicit FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance);

	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;

	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;

	virtual void Update(float DeltaSeconds) override;

	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;

	void CalculateLeanAmount(UPGGuardianAnimData* Data, const FRotator& InCurrentRotation, float DeltaSeconds);
};

UCLASS()
class PLANETGUARDIAN_API UPGGuardianAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	UPGGuardianAnimData* BackAnimData;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UPGGuardianAnimData* AnimData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UPGCameraShake> CameraShake;

	bool bUsingFrontAnimData { false };

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

private:
	UFUNCTION()
	void AnimNotify_ShakeCameraOnLand();

	friend void FPGGuardianAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const;

	friend struct FPGGuardianAnimInstanceProxy;
};
