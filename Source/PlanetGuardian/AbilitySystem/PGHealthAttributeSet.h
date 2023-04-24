// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PGHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGHealthAttributeSet : public UPGAttributeSet
{
	GENERATED_BODY()

public:
	UPGHealthAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPGHealthAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category="Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPGHealthAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category="Meta|Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPGHealthAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category="Meta|Healing")
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UPGHealthAttributeSet, Healing);

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
