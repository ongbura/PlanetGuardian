// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

static struct FPGInputTags : FGameplayTagNativeAdder
{
	inline static FGameplayTag Input_Action_Move{};
	inline static FGameplayTag Input_Action_Look{};
	inline static FGameplayTag Input_Action_Jump{};

	virtual void AddTags() override;
} InputTags;
