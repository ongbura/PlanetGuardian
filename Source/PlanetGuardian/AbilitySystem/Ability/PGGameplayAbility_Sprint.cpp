// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility_Sprint.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"

UPGGameplayAbility_Sprint::UPGGameplayAbility_Sprint()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UPGGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bShouldSprint = true;

	if (SprintEffectClass != nullptr)
	{
		const auto* Effect = SprintEffectClass->GetDefaultObject<UGameplayEffect>();
		SprintEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Effect, GetSystemGlobalLevel());
	}

	// TODO: Add GameplayCue to owner which removed on ability end.

	auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent);
	check(CMC);
	
	CMC->Sprint();
}

bool UPGGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!ActorInfo || !ActorInfo->MovementComponent.IsValid())
	{
		return false;
	}

	const auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent.Get());
	check(CMC);

	return CMC->CanSprint();
}

void UPGGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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
	check(CMC);
	
	CMC->StopSprinting();
}
