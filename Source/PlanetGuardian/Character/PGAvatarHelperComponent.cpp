// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAvatarHelperComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

UPGAvatarHelperComponent::UPGAvatarHelperComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;	
}

void UPGAvatarHelperComponent::InitializeAbilitySystem(UPGAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystem.Get() == InASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	if (AbilitySystem.IsValid())
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}

	AbilitySystem = InASC;
	AbilitySystem->InitAbilityActorInfo(InOwnerActor, GetPawnChecked<APawn>());

	OnAbilitySystemInitialized.Broadcast();
}

void UPGAvatarHelperComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystem.IsValid())
	{
		return;
	}

	if (AbilitySystem->GetAvatarActor() == GetOwner())
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		// AbilityTypesToIgnore.AddTag(FLyraGameplayTags::Get().Ability_Behavior_SurvivesDeath);

		AbilitySystem->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		// TODO: AbilitySystem->ClearAbilityInput();
		AbilitySystem->RemoveAllGameplayCues();

		AbilitySystem->SetAvatarActor(nullptr);

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystem.Reset();
}

// Called when the game starts
void UPGAvatarHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPGAvatarHelperComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	
	Super::EndPlay(EndPlayReason);
}

void UPGAvatarHelperComponent::OnAvatarControllerChanged()
{
	bSetController = true;
}

void UPGAvatarHelperComponent::OnAvatarPlayerStateChanged()
{
	bSetPlayerState = true;
}


