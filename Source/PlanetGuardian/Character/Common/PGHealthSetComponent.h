// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PGAttributeSetComponent.h"
#include "PGHealthSetComponent.generated.h"

UENUM(BlueprintType)
enum class EPGDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGHealthSetComponent : public UPGAttributeSetComponent
{
	GENERATED_BODY()

public:
	FPGOnAttributeChanged OnHealthChanged;
	FPGOnAttributeChanged OnMaxHealthChanged;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="0", UIMax="100", ForceUnits="Percent"))
	float InitialHealthPercent { 100.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="1", ClampMin="1"))
	float DefaultMaxHealth { 100.f };

	UPROPERTY(ReplicatedUsing=OnRep_DeathState)
	EPGDeathState DeathState;
	
public:
	UPGHealthSetComponent();

	float GetHealth() const;

	float GetMaxHealth() const;

	float GetHealthNormalized() const;
	
protected:
	virtual void InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC) override;

	virtual void UninitializeFromAbilitySystem() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	
	void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	// Begins the death sequence for the owner.
	void StartDeath();

	// Ends the death sequence for the owner.
	void FinishDeath();
	
	UFUNCTION()
	void OnRep_DeathState(EPGDeathState OldDeathState);
};
