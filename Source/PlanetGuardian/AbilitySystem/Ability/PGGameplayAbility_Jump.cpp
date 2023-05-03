// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility_Jump.h"
#include "PGNativeGameplayTags.h"
#include "Character/PlayableCharacter/PGPlayableCharacter.h"

UPGGameplayAbility_Jump::UPGGameplayAbility_Jump()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UPGGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

		Jump();
	}
}

bool UPGGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const auto* Avatar = Cast<APGPlayableCharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar || !Avatar->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UPGGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPGGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	StopJumping();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGGameplayAbility_Jump::Jump()
{
	if (auto* Avatar = Cast<APGPlayableCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (Avatar->IsLocallyControlled() && !Avatar->bPressedJump)
		{
			Avatar->UnCrouch();
			Avatar->Jump();
		}
	}
}

void UPGGameplayAbility_Jump::StopJumping()
{
	if (auto* Avatar = Cast<APGPlayableCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (Avatar->IsLocallyControlled() && Avatar->bPressedJump)
		{
			Avatar->StopJumping();
		}
	}
}
