// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "PGTaggedTypeManager.generated.h"

class UPGTaggedInputAction;
class UPGTaggedAbility;

UCLASS()
class PLANETGUARDIAN_API UPGTaggedTypeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FGameplayTag, class UInputAction*> InputActions;

	UPROPERTY()
	TMap<FGameplayTag, class UPGGameplayAbility*> Abilities;
	
public:
	static UPGTaggedTypeManager* Get();
	
	UInputAction* FindInputAction(const FGameplayTag& Tag) const;

	UPGGameplayAbility* FindAbility(const FGameplayTag& Tag) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
