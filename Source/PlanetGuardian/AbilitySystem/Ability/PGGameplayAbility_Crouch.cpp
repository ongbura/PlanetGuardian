// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility_Crouch.h"
#include "Character/Guardian/PGGuardian.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"

void UPGGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent);
		check(CMC);

		CMC->ToggleCrouch();
		
		// auto* Avatar = Cast<APGGuardian>(ActorInfo->AvatarActor.Get());
		// check(Avatar);
		//
		// if (Avatar->bIsCrouched)
		// {
		// 	Avatar->UnCrouch(true);
		// }
		// else
		// {
		// 	Avatar->Crouch(true);
		// }
	}
}

bool UPGGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent);
	check(CMC);

	if (CMC->IsFalling())
	{
		return false;
	}
	
	if (!CMC->IsCrouching() && CMC->CanCrouchInCurrentState())
	{
		return true;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UPGGameplayAbility_Crouch::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

}

void UPGGameplayAbility_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
