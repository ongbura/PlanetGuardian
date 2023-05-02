// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/PGGameplayAbility.h"
#include "PGInputActionAbilitySet.generated.h"

class UInputAction;
class UPGGameplayAbility;

UCLASS()
class PLANETGUARDIAN_API UPGInputActionAbilityData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TMap<TSoftObjectPtr<UInputAction>, TSoftClassPtr<UPGGameplayAbility>> InputActionAbilityMap;
	
public:
	const auto& GetInputActionAbilityMap() const { return InputActionAbilityMap; }

	template <typename FuncType>
	void ForEachLoadedInputActionAbilitySet(FuncType Func) const;
	
};

template <typename FuncType>
void UPGInputActionAbilityData::ForEachLoadedInputActionAbilitySet(FuncType Func) const
{
	for (const auto& InputActionAbilityPair : InputActionAbilityMap)
	{
		auto* InputAction = InputActionAbilityPair.Key.LoadSynchronous();
		auto* Ability = Cast<UPGGameplayAbility>(InputActionAbilityPair.Value.LoadSynchronous()->GetDefaultObject());

		Func(InputAction, Ability);
	}
}
