// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

static struct FPGNativeGameplayTags : FGameplayTagNativeAdder
{
    // Attribute.Common
	inline static FGameplayTag Attribute_Common_Health{};
	
	// Input.Action
	inline static FGameplayTag Input_Action_Jump{};
	inline static FGameplayTag Input_Action_Look{};
	inline static FGameplayTag Input_Action_Move{};

    virtual void AddTags() override;
} GNativeTags;
