// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PGTaggedData.generated.h"

UCLASS()
class PLANETGUARDIAN_API UPGTaggedInputAction : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, TSoftObjectPtr<class UInputAction>> InputActions;

public:
	const auto& GetInputActions() const { return InputActions; }
};

UCLASS()
class PLANETGUARDIAN_API UPGTaggedAbility : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, TSoftObjectPtr<class UPGGameplayAbility>> Abilities;

public:
	const auto& GetAbilities() const { return Abilities; }
};

