// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PGDefaultAbilityConfig.generated.h"

class UPGGameplayAbility;

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGDefaultAbilityConfig : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="Ability", AllowPrivateAccess="true"))
	TArray<TSubclassOf<UPGGameplayAbility>> DefaultAbilities;

public:
	const auto& GetDefaultAbilities() const { return DefaultAbilities; }
};
