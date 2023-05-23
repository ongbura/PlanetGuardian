// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayAbility_Sprint.h"

#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Guardian/PGGuardian.h"
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

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent.Get());
		if (CMC == nullptr)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		CMC->Sprint();

		if (SprintEffectClass != nullptr)
		{
			const auto* Effect = SprintEffectClass->GetDefaultObject<UGameplayEffect>();
			SprintEffectHandle = ApplyGameplayEffectToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),
															GetCurrentActivationInfo(), Effect,
															GetSystemGlobalLevel());
		}

		UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);

		WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnSprintReleased);
		WaitInputReleaseTask->ReadyForActivation();

		CommitSprintAbilityLoop();
	}
}

bool UPGGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanSprint = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	const auto* CMC = Cast<UPGGuardianMovementComponent>(ActorInfo->MovementComponent.Get());
	if (CMC == nullptr)
	{
		bCanSprint &= false;
	}
	else
	{
		bCanSprint &= CMC->CanSprint();
	}
	
	return bCanSprint;
}

void UPGGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	if (const auto* Avatar = Cast<APGGuardian>(ActorInfo->AvatarActor.Get()))
	{
		if (SprintEffectHandle.IsValid())
		{
			BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);
		}

		auto* CMC = Cast<UPGGuardianMovementComponent>(Avatar->GetMovementComponent());
		check(CMC);

		CMC->StopSprinting();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);	
}

void UPGGameplayAbility_Sprint::OnSprintReleased(float TimeHeld)
{
	bShouldTerminateCommittingLoop = true;

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPGGameplayAbility_Sprint::CommitSprintAbilityLoop()
{
	if (bShouldTerminateCommittingLoop)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.05f);
	WaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnWaitingFinishedForCommitSprint);
	
	WaitDelayTask->ReadyForActivation();	
}

void UPGGameplayAbility_Sprint::OnWaitingFinishedForCommitSprint()
{
	UAbilityTask_NetworkSyncPoint* SyncPointTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyServerWait);
	SyncPointTask->OnSync.AddDynamic(this, &ThisClass::OnServerSyncedForCommitSprint);

	SyncPointTask->ReadyForActivation();
}

void UPGGameplayAbility_Sprint::OnServerSyncedForCommitSprint()
{
	if (bShouldTerminateCommittingLoop)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	
	const auto* CMC = Cast<UPGGuardianMovementComponent>(GetCurrentActorInfo()->MovementComponent.Get());
	check(CMC);

	const bool bHasEnoughPower = CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());

	if (CMC->CanSprint() && bHasEnoughPower)
	{
		CommitSprintAbilityLoop();
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}	
}
