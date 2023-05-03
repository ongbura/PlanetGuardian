// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PGAttributeSetComponent.h"
#include "PGJetpackPowerSetComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGJetpackPowerSetComponent : public UPGAttributeSetComponent
{
	GENERATED_BODY()

public:
	FPGOnAttributeChanged OnJetpackPowerChanged;
	FPGOnAttributeChanged OnMaxJetpackPowerChanged;
	FPGOnAttributeChanged OnJetpackPowerRegenRateChanged;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="0", UIMax="100", ForceUnits="Percent"))
	float InitialJetpackPowerPercent { 70.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float DefaultMaxJetpackPower { 20.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float DefaultJetpackPowerRegenRate { 0.5f };

public:
	UPGJetpackPowerSetComponent();

	float GetJetpackPower() const;

	float GetMaxJetpackPower() const;

	float GetJetpackPowerRegenRateChanged() const;

protected:
	virtual void InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC) override;

	virtual void UninitializeFromAbilitySystem() override;

private:
	void HandleJetpackPowerChanged(const FOnAttributeChangeData& ChangeData);

	void HandleMaxJetpackPowerChanged(const FOnAttributeChangeData& ChangeData);

	void HandleJetpackPowerRegenRateChanged(const FOnAttributeChangeData& ChangeData);

	

};
