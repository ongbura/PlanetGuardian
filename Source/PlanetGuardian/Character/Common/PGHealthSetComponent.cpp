// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGHealthSetComponent.h"

#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "Net/UnrealNetwork.h"


UPGHealthSetComponent::UPGHealthSetComponent()
{
	SetIsReplicatedByDefault(true);
}

const UPGHealthSet* UPGHealthSetComponent::GetHealthSet() const
{
	return GetAttributeSet<UPGHealthSet>();
}

float UPGHealthSetComponent::GetHealth() const
{
	const auto* HealthSet = GetHealthSet();
	return HealthSet->GetHealth();
}

float UPGHealthSetComponent::GetMaxHealth() const
{
	const auto* HealthSet = GetHealthSet();
	return HealthSet->GetMaxHealth();
}

float UPGHealthSetComponent::GetHealthRegenRate() const
{
	const auto* HealthSet = GetHealthSet();
	return HealthSet->GetHealthRegenRate();
}

void UPGHealthSetComponent::InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC)
{
	Super::InitializeWithAbilitySystem(InASC);

	auto* HealthSet = InASC->GetSet<UPGHealthSet>();
	check(HealthSet);

	InASC->SetNumericAttributeBase(HealthSet->GetMaxHealthAttribute(), DefaultMaxHealth);
	InASC->SetNumericAttributeBase(HealthSet->GetHealthAttribute(), DefaultMaxHealth * InitialHealthPercent / 100.f);

	AttributeSet = HealthSet;
	check(AttributeSet.IsValid());
}

void UPGHealthSetComponent::UninitializeFromAbilitySystem()
{
	Super::UninitializeFromAbilitySystem();
}

void UPGHealthSetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPGHealthSetComponent, DeathState);
}

void UPGHealthSetComponent::StartDeath()
{
}

void UPGHealthSetComponent::FinishDeath()
{
}

void UPGHealthSetComponent::OnRep_DeathState(EPGDeathState OldDeathState)
{
}


