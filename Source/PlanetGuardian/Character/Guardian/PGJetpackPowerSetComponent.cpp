// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGJetpackPowerSetComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"


UPGJetpackPowerSetComponent::UPGJetpackPowerSetComponent()
{
}

float UPGJetpackPowerSetComponent::GetJetpackPower() const
{
	check(AttributeSet.IsValid());
	
	auto* JetpackPowerSet = Cast<UPGJetpackPowerSet>(AttributeSet.Get());
	check(JetpackPowerSet);

	return JetpackPowerSet->GetJetpackPower();
}

float UPGJetpackPowerSetComponent::GetMaxJetpackPower() const
{
	check(AttributeSet.IsValid());
	
	auto* JetpackPowerSet = Cast<UPGJetpackPowerSet>(AttributeSet.Get());
	check(JetpackPowerSet);

	return JetpackPowerSet->GetMaxJetpackPower();
}

float UPGJetpackPowerSetComponent::GetJetpackPowerRegenRateChanged() const
{
	check(AttributeSet.IsValid());
	
	auto* JetpackPowerSet = Cast<UPGJetpackPowerSet>(AttributeSet.Get());
	check(JetpackPowerSet);

	return JetpackPowerSet->GetJetpackPowerRegenRate();
}

void UPGJetpackPowerSetComponent::InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC)
{
	Super::InitializeWithAbilitySystem(InASC);

	AttributeSet = InASC->GetSet<UPGJetpackPowerSet>();
	check(AttributeSet.IsValid());

	InASC->GetGameplayAttributeValueChangeDelegate(UPGJetpackPowerSet::GetJetpackPowerAttribute()).AddUObject(this, &ThisClass::HandleJetpackPowerChanged);
	InASC->GetGameplayAttributeValueChangeDelegate(UPGJetpackPowerSet::GetMaxJetpackPowerAttribute()).AddUObject(this, &ThisClass::HandleMaxJetpackPowerChanged);
	InASC->GetGameplayAttributeValueChangeDelegate(UPGJetpackPowerSet::GetJetpackPowerRegenRateAttribute()).AddUObject(this, &ThisClass::HandleJetpackPowerRegenRateChanged);

	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetMaxJetpackPowerAttribute(), DefaultMaxJetpackPower);
	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetJetpackPowerAttribute(), InitialJetpackPowerPercent * DefaultMaxJetpackPower / 100.f);
	InASC->SetNumericAttributeBase(UPGJetpackPowerSet::GetJetpackPowerRegenRateAttribute(), DefaultJetpackPowerRegenRate);
}

void UPGJetpackPowerSetComponent::UninitializeFromAbilitySystem()
{
	Super::UninitializeFromAbilitySystem();
}

void UPGJetpackPowerSetComponent::HandleJetpackPowerChanged(const FOnAttributeChangeData& ChangeData)
{
	if (OnJetpackPowerChanged.IsBound())
	{
		OnJetpackPowerChanged.Broadcast(AbilitySystemComponent.Get(), GetInstigatorFromAttrChangeDate(ChangeData), ChangeData.NewValue, ChangeData.OldValue);
	}
}

void UPGJetpackPowerSetComponent::HandleMaxJetpackPowerChanged(const FOnAttributeChangeData& ChangeData)
{
	if (OnMaxJetpackPowerChanged.IsBound())
	{
		OnMaxJetpackPowerChanged.Broadcast(AbilitySystemComponent.Get(), GetInstigatorFromAttrChangeDate(ChangeData), ChangeData.NewValue, ChangeData.OldValue);
	}
}

void UPGJetpackPowerSetComponent::HandleJetpackPowerRegenRateChanged(const FOnAttributeChangeData& ChangeData)
{
	if (OnJetpackPowerRegenRateChanged.IsBound())
	{
		OnJetpackPowerRegenRateChanged.Broadcast(AbilitySystemComponent.Get(), GetInstigatorFromAttrChangeDate(ChangeData), ChangeData.NewValue, ChangeData.OldValue);
	}
}



