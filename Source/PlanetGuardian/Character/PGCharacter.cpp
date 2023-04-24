// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGCharacter.h"
#include "PGAvatarHelperComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Player/PGPlayerState.h"

APGCharacter::APGCharacter()
{
	AvatarHelper = CreateDefaultSubobject<UPGAvatarHelperComponent>(TEXT("Avater Helper"));

	if (GetNetMode() == NM_Client)
	{
		OnControllerChanged.AddUObject(AvatarHelper, &UPGAvatarHelperComponent::OnAvatarControllerChanged);
		OnPlayerStateChanged.AddUObject(AvatarHelper, &UPGAvatarHelperComponent::OnAvatarPlayerStateChanged);
	}
}

UAbilitySystemComponent* APGCharacter::GetAbilitySystemComponent() const
{
	if (AbilitySystem.IsValid())
	{
		return AbilitySystem.Get();
	}

	return nullptr;
}

void APGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init AbilitySystem on Server Side.

	if (auto* PS = GetPlayerStateChecked<APGPlayerState>())
	{
		AbilitySystem = PS->GetAbilitySystem();
		AvatarHelper->InitializeAbilitySystem(AbilitySystem.Get(), PS);
	}
}

void APGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init AbilitySystem on Client Side.

	if (auto* PS = GetPlayerStateChecked<APGPlayerState>())
	{
		AbilitySystem = PS->GetAbilitySystem();
		AvatarHelper->InitializeAbilitySystem(AbilitySystem.Get(), PS);

		// TODO: Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.

		OnPlayerStateChanged.Broadcast();
	}
}
