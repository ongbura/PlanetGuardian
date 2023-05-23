// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PGAttributeSetComponent.h"
#include "PGJetpackPowerSetComponent.generated.h"


class UPGJetpackPowerSet;

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGJetpackPowerSetComponent : public UPGAttributeSetComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="0", UIMax="100", ForceUnits="Percent"))
	float InitialJetpackPowerPercent { 100.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float DefaultMaxJetpackPower { 50.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float DefaultJetpackPowerRegenRate { 0.5f };

public:
	UPGJetpackPowerSetComponent();

	const UPGJetpackPowerSet* GetJetpackPowerSet() const;

	float GetJetpackPower() const;

	float GetMaxJetpackPower() const;

	float GetJetpackPowerRegenRateChanged() const;

protected:
	virtual void InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC) override;

	virtual void UninitializeFromAbilitySystem() override;
};
