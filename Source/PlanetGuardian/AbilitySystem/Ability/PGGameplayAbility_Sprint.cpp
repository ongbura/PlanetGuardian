// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility_Sprint.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"

void UPGGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bShouldSprint = true;

	if (SprintEffectClass->IsValidLowLevel())
	{
		const auto* Effect = SprintEffectClass->GetDefaultObject<UGameplayEffect>();
		SprintEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Effect, GetSystemGlobalLevel());
	}

	// TODO: Add GameplayCue to owner which removed on ability end.

	auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent);
	CMC->Sprint();

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UPGGameplayAbility_Sprint::OnSprintKeyReleased);
	WaitInputReleaseTask->ReadyForActivation();
}

void UPGGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (SprintEffectHandle.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);
	}

	auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent);
	CMC->StopSprinting();
}

void UPGGameplayAbility_Sprint::OnSprintKeyReleased(float TimeHeld)
{
	bShouldSprint = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
