// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGHealthAttributeSet.h"
#include "PGNativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

UPGHealthAttributeSet::UPGHealthAttributeSet()
{
	AttributeSetTag = GNativeTags.Attribute_Common_Health;
}

void UPGHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetMaxHealthAttribute());
	}
}

void UPGHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UPGHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPGHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPGHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPGHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGHealthAttributeSet, Health, OldHealth);
}

void UPGHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPGHealthAttributeSet, MaxHealth, OldMaxHealth);
}
