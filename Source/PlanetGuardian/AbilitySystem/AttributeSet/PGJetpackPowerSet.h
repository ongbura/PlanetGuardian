// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PGAttributeSet.h"
#include "PGJetpackPowerSet.generated.h"


UCLASS()
class PLANETGUARDIAN_API UPGJetpackPowerSet final : public UPGAttributeSet
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_JetpackPower, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData JetpackPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxJetpackPower, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData MaxJetpackPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_JetpackPowerRegenRate, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData JetpackPowerRegenRate;

public:
	UPGJetpackPowerSet();

	ATTRIBUTE_ACCESSORS(UPGJetpackPowerSet, JetpackPower);
	ATTRIBUTE_ACCESSORS(UPGJetpackPowerSet, MaxJetpackPower);
	ATTRIBUTE_ACCESSORS(UPGJetpackPowerSet, JetpackPowerRegenRate);

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	UFUNCTION()
	void OnRep_JetpackPower(const FGameplayAttributeData& OldJetpackPower);

	UFUNCTION()
	void OnRep_MaxJetpackPower(const FGameplayAttributeData& OldMaxJetpackPower);

	UFUNCTION()
	void OnRep_JetpackPowerRegenRate(const FGameplayAttributeData& OldJetpackPowerRegenRate);
};
