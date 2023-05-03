// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGHealthSetComponent.h"

#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "Net/UnrealNetwork.h"


UPGHealthSetComponent::UPGHealthSetComponent()
{
	SetIsReplicatedByDefault(true);
}

float UPGHealthSetComponent::GetHealth() const
{
	check(AttributeSet.IsValid());
	
	const auto* HealthSet = Cast<UPGHealthSet>(AttributeSet.Get());
	check(HealthSet);

	return HealthSet->GetHealth();
}

float UPGHealthSetComponent::GetMaxHealth() const
{
	check(AttributeSet.IsValid());

	const auto* HealthSet = Cast<UPGHealthSet>(AttributeSet.Get());
	check(HealthSet);

	return HealthSet->GetMaxHealth();
}

float UPGHealthSetComponent::GetHealthNormalized() const
{
	const float Health = GetHealth();
	const float MaxHealth = GetMaxHealth();

	return MaxHealth > 0.f ? Health / MaxHealth : 0.f;
}

void UPGHealthSetComponent::InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC)
{
	Super::InitializeWithAbilitySystem(InASC);

	AttributeSet = InASC->GetSet<UPGHealthSet>();
	check(AttributeSet.IsValid());

	InASC->GetGameplayAttributeValueChangeDelegate(UPGHealthSet::GetHealthAttribute()).AddUObject(this, &UPGHealthSetComponent::HandleHealthChanged);
	InASC->GetGameplayAttributeValueChangeDelegate(UPGHealthSet::GetMaxHealthAttribute()).AddUObject(this, &UPGHealthSetComponent::HandleMaxHealthChanged);

	auto* HealthSet = Cast<UPGHealthSet>(AttributeSet.Get());
	check(HealthSet);

	InASC->SetNumericAttributeBase(HealthSet->GetMaxHealthAttribute(), DefaultMaxHealth);
	InASC->SetNumericAttributeBase(HealthSet->GetHealthAttribute(), DefaultMaxHealth * InitialHealthPercent / 100.f);
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

void UPGHealthSetComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	check(AbilitySystemComponent.IsValid());
	
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(AbilitySystemComponent.Get(), GetInstigatorFromAttrChangeDate(ChangeData), ChangeData.NewValue, ChangeData.OldValue);
	}
}

void UPGHealthSetComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	check(AbilitySystemComponent.IsValid());
	
	if (OnMaxHealthChanged.IsBound())
	{
		OnMaxHealthChanged.Broadcast(AbilitySystemComponent.Get(), GetInstigatorFromAttrChangeDate(ChangeData), ChangeData.NewValue, ChangeData.OldValue);
	}
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


