// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PGInputActionAbilitySet.generated.h"

class UInputAction;
class UPGGameplayAbility;

USTRUCT()
struct FPGInputActionAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UPGGameplayAbility> AbilityClass;
	
	UInputAction* LoadInputAction() const;

	UPGGameplayAbility* LoadGameplayAbility() const;
};

UCLASS()
class PLANETGUARDIAN_API UPGInputActionAbilityData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TArray<FPGInputActionAbilitySet> InputActionAbilityData;
	
public:
	const auto& GetInputActionAbilityData() const { return InputActionAbilityData; }
};
