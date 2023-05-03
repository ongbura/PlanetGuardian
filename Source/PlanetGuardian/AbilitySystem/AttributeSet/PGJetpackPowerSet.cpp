// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGJetpackPowerSet.h"
#include "PGNativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

UPGJetpackPowerSet::UPGJetpackPowerSet()
{
	AttributeSetTag = GNativeTags.Attribute_Guardian_Movement;
}

void UPGJetpackPowerSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPGJetpackPowerSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPGJetpackPowerSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxJetpackPowerAttribute())
	{
		if (GetJetpackPower() > NewValue)
		{
			auto* ASC = GetOwningAbilitySystemComponent();
			ASC->ApplyModToAttribute(GetJetpackPowerAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UPGJetpackPowerSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetJetpackPowerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxJetpackPower());
	}
	else if (Attribute == GetMaxJetpackPowerAttribute())
	{
		static constexpr float MinMaxJetpackPower = 1.f;
		NewValue = FMath::Max(MinMaxJetpackPower, NewValue);
	}
}

void UPGJetpackPowerSet::OnRep_JetpackPower(const FGameplayAttributeData& OldJetpackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGJetpackPowerSet, JetpackPower, OldJetpackPower);
}

void UPGJetpackPowerSet::OnRep_MaxJetpackPower(const FGameplayAttributeData& OldMaxJetpackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGJetpackPowerSet, MaxJetpackPower, OldMaxJetpackPower);
}

void UPGJetpackPowerSet::OnRep_JetpackPowerRegenRate(const FGameplayAttributeData& OldJetpackPowerRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGJetpackPowerSet, JetpackPowerRegenRate, OldJetpackPowerRegenRate);
}

void UPGJetpackPowerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPGJetpackPowerSet, JetpackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPGJetpackPowerSet, MaxJetpackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPGJetpackPowerSet, JetpackPowerRegenRate, COND_None, REPNOTIFY_Always);
}
