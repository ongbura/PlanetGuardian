// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGHealthSet.h"
#include "PGNativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

UPGHealthSet::UPGHealthSet()
{
	AttributeSetTag = GNativeTags.Attribute_Common_Health;
}

void UPGHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPGHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPGHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPGHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPGHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPGHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		auto* ASC = GetOwningAbilitySystemComponent();
		ASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
	}
}

void UPGHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		static constexpr float MinMaxHealth = 1.f;
		NewValue = FMath::Max(NewValue, MinMaxHealth);
	}
}

void UPGHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGHealthSet, Health, OldHealth);
}

void UPGHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGHealthSet, MaxHealth, OldMaxHealth);
}
