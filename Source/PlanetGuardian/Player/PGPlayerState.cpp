// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayerState.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

APGPlayerState::APGPlayerState()
{
	AbilitySystem = CreateDefaultSubobject<UPGAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystem->SetIsReplicated(true);

	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	
}

UAbilitySystemComponent* APGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

// Called when the game starts or when spawned
void APGPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}