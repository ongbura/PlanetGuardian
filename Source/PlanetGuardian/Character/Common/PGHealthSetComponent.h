// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PGAttributeSetComponent.h"
#include "PGHealthSetComponent.generated.h"

class UPGHealthSet;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="0", UIMax="100", ForceUnits="Percent"))
	float InitialHealthPercent { 100.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true", UIMin="1", ClampMin="1"))
	float DefaultMaxHealth { 100.f };

	UPROPERTY(ReplicatedUsing=OnRep_DeathState)
	EPGDeathState DeathState;
	
public:
	UPGHealthSetComponent();

	const UPGHealthSet* GetHealthSet() const;

	float GetHealth() const;

	float GetMaxHealth() const;

	float GetHealthRegenRate() const;

protected:
	virtual void InitializeWithAbilitySystem(UPGAbilitySystemComponent* InASC) override;

	virtual void UninitializeFromAbilitySystem() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Begins the death sequence for the owner.
	void StartDeath();

	// Ends the death sequence for the owner.
	void FinishDeath();
	
	UFUNCTION()
	void OnRep_DeathState(EPGDeathState OldDeathState);
};
