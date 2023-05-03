// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayerState.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"

APGPlayerState::APGPlayerState()
{
	AbilitySystem = CreateDefaultSubobject<UPGAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystem->SetIsReplicated(true);

	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<UPGHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UPGJetpackPowerSet>(TEXT("JetpackPowerSet"));
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