// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

static struct FPGNativeGameplayTags : FGameplayTagNativeAdder
{
    // Ability.Guardian
	FGameplayTag Ability_Guardian_Movement_Jump{};
	FGameplayTag Ability_Guardian_Movement_Sprint{};
	
	// Attribute.Common
	FGameplayTag Attribute_Common_Health{};
	
	// Attribute.Guardian
	FGameplayTag Attribute_Guardian_JetpackPower{};

    virtual void AddTags() override;
} GNativeTags;
