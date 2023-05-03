// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAttributeSet.h"
#include "AbilitySystemComponent.h"

void UPGAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                  const FGameplayAttributeData& MaxAttribute, const float NewMaxValue,
                                                  const FGameplayAttribute& AffectedAttributeProperty) const
{
	auto* const AbilitySystem = GetOwningAbilitySystemComponent();
	check(AbilitySystem);

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue))
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = CurrentMaxValue > 0.f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilitySystem->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UPGAttributeSet::InitializeAttributeForMax(const FGameplayAttribute& Attribute, const FGameplayAttributeData& MaxAttributeData) const
{
	auto* const AbilitySystem = GetOwningAbilitySystemComponent();
	check(AbilitySystem);

	AbilitySystem->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Override, MaxAttributeData.GetCurrentValue());
}