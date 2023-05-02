// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayableCharacter/PGPlayableCharacter.h"
#include "PGGuardian.generated.h"

class UPGGuardianMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
class UAudioComponent;
class UCurveFloat;
class UPGAbilitySystemComponent;
class UPGHealthAttributeSet;

UCLASS()
class PLANETGUARDIAN_API APGGuardian final : public APGPlayableCharacter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPGGuardianMovementComponent> GuardianMovementComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> Jetpack;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraComponent> JetpackEffect;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> JetpackSoundEffect;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPGHealthAttributeSet> HealthAttributeSet;

	bool bIsJetpackActivated{ false };
	float ThrusterTime{ 0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCurveFloat> JetpackBoostCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	float ThrusterMaxTime{ 2.f};

public:
	explicit APGGuardian(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * @brief The jetpack can be started, stopped and reset here.
	 * This is called by the jump input when in air or reset when landed.
	 * The jetpack and its functionality could become an own component if you would have several characters able to use it.
	 */
	void ToggleJetpack(bool bReset, bool bActivate);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	/**
	 * @brief Update CameraBoom TargetArmLength
	 */
	void UpdateCamera(float DeltaSeconds);
	
	/**
	 * @brief Update Jetpack, called from tick function to decrease the thruster time. Using a curve asset to get the Z force applied on the character.
	 */
	void UpdateJetpack(float DeltaSeconds);

	FORCEINLINE void ResetThrusterTime() { ThrusterTime = ThrusterMaxTime; }

	UFUNCTION()
	void OnLandedToggleJetpack(const FHitResult& Hit);
};
