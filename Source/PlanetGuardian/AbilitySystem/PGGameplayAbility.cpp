// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility.h"
#include "AbilitySystemComponent.h"

UPGGameplayAbility::UPGGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// This option causes trouble more often than not.
	// It means if the client's GameplayAbility ends either due to cancellation or natural completion,
	// it will force the server's version to end whether it completed or not.
	// The latter issue is the important one, especially for locally predicted GameplayAbilities used by players with high latencies.
	// Generally you will want to disable this option.
	bServerRespectsRemoteAbilityCancellation = false;

	// Setting this option will always replicate input press and release events to the server.
	// Epic recommends not using this
	// and instead relying on the Generic Replicated Events that are built into the existing input related AbilityTasks if you have your input bound to your ASC.
	bReplicateInputDirectly = false;
}

void UPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bShouldActivateOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
