// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "PGAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	FGameplayTag AttributeSetTag;

public:
	FORCEINLINE const FGameplayTag& GetAttributeSetTag() const { return AttributeSetTag; }

protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;

	// Helper function to initialize an attribute's value to match the value of its associated max attribute.
	void InitializeAttributeForMax(const FGameplayAttribute& Attribute, const FGameplayAttributeData& MaxAttributeData) const;
};
