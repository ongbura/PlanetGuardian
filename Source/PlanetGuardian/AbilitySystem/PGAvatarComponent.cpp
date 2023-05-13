// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAvatarComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Multiplayer/PGPlayerState.h"
#include "AbilitySystem/Ability/PGGameplayAbility.h"

UPGAvatarComponent::UPGAvatarComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UPGAvatarComponent::InitializeAbilitySystem(UPGAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent.Get() == InASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	if (AbilitySystemComponent.IsValid())
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, GetPawnChecked<APawn>());

	OnAbilitySystemInitialized.Broadcast(InASC);
}

void UPGAvatarComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		// AbilityTypesToIgnore.AddTag(FLyraGameplayTags::Get().Ability_Behavior_SurvivesDeath);

		AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		// TODO: AbilitySystem->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		AbilitySystemComponent->SetAvatarActor(nullptr);
	}

	AbilitySystemComponent.Reset();
	OnAbilitySystemUninitialized.Broadcast();
}

void UPGAvatarComponent::HandlePlayerControllerAssigned()
{
	if (AbilitySystemComponent.IsValid() && AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>())
	{
		ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
		if (AbilitySystemComponent->GetOwnerActor() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			// Solves the data-races of controller / playerstate
			AbilitySystemComponent->RefreshAbilityActorInfo();
		}
	}

	bControllerAssigned = true;

	OnPlayerControllerAssigned.Broadcast(GetController<APGPlayerController>());
}

void UPGAvatarComponent::HandlePlayerStateAssigned()
{
	if (AbilitySystemComponent.IsValid() && AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>())
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
	else
	{
		if (auto* PS = GetPlayerState<APGPlayerState>())
		{
			InitializeAbilitySystem(PS->GetPGAbilitySystemComponent(), PS);
		}
	}

	bPlayerStateAssigned = true;
}

// Called when the game starts
void UPGAvatarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPGAvatarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();

	Super::EndPlay(EndPlayReason);
}