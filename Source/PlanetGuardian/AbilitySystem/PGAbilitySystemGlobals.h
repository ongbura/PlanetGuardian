// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PGAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	static constexpr int32 SystemGlobalLevel { 1 };
};
