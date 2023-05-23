// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PGHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGHealthSet : public UPGAttributeSet
{
	GENERATED_BODY()

	static constexpr float MinHealth = 0.f;
	
	static constexpr float MinMaxHealth = 1.f;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegenRate, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData HealthRegenRate;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Healing;

public:
	UPGHealthSet();
	
	ATTRIBUTE_ACCESSORS(UPGHealthSet, Health);	
	ATTRIBUTE_ACCESSORS(UPGHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPGHealthSet, HealthRegenRate);
	ATTRIBUTE_ACCESSORS(UPGHealthSet, Damage);	
	ATTRIBUTE_ACCESSORS(UPGHealthSet, Healing);

	float GetHealthBase() const { return Health.GetBaseValue(); }
	float GetMaxHealthBase() const { return MaxHealth.GetBaseValue(); }

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;	

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
