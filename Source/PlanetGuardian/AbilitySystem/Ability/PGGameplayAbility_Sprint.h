﻿// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameplayAbility.h"
#include "PGGameplayAbility_Sprint.generated.h"

class UGameplayEffect;

UCLASS(Abstract)
class PLANETGUARDIAN_API UPGGameplayAbility_Sprint : public UPGGameplayAbility
{
	GENERATED_BODY()

	bool bShouldSprint { false };

	FActiveGameplayEffectHandle SprintEffectHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> SprintEffectClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnSprintKeyReleased(float TimeHeld);
};
