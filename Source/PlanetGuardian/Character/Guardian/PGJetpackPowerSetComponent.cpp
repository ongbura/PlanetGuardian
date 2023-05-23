// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGJetpackPowerSetComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"

UPGJetpackPowerSetComponent::UPGJetpackPowerSetComponent()
{
}

const UPGJetpackPowerSet* UPGJetpackPowerSetComponent::GetJetpackPowerSet() const
{
	return GetAttributeSet<UPGJetpackPowerSet>();
}

float UPGJetpackPowerSetComponent::GetJetpackPower() const
{
	const auto* JetpackPowerSet = GetJetpackPowerSet();
	return JetpackPowerSet->GetJetpackPower();
}

float UPGJetpackPowerSetComponent::GetMaxJetpackPower() const
{
	const auto* JetpackPowerSet = GetJetpackPowerSet();
	return JetpackPowerSet->GetMaxJetpackPower();
}

float UPGJetpackPowerSetComponent::GetJetpackPowerRegenRateChanged() const
{
	const auto* JetpackPowerSet = GetJetpackPowerSet();
	return JetpackPowerSet->GetJetpackPowerRegenRate();
}

void UPGJetpackPowerSetComponent::InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC)
{
	Super::InitializeWithAbilitySystem(InASC);

	AttributeSet = InASC->GetSet<UPGJetpackPowerSet>();
	check(AttributeSet.IsValid());

	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetMaxJetpackPowerAttribute(), DefaultMaxJetpackPower);
	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetJetpackPowerAttribute(), InitialJetpackPowerPercent * DefaultMaxJetpackPower / 100.f);
	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetJetpackPowerRegenRateAttribute(), DefaultJetpackPowerRegenRate);
}

void UPGJetpackPowerSetComponent::UninitializeFromAbilitySystem()
{
	Super::UninitializeFromAbilitySystem();
}



