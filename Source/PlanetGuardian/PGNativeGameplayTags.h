// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

static struct FPGNativeGameplayTags : FGameplayTagNativeAdder
{
    // Ability.Crouch
	FGameplayTag Ability_Crouch{};
	
	// Ability.Jump
	FGameplayTag Ability_Jump{};
	
	// Attribute.Common
	FGameplayTag Attribute_Common_Health{};
	
	// Input.Action
	FGameplayTag Input_Action_Jump{};
	FGameplayTag Input_Action_Look{};
	FGameplayTag Input_Action_Move{};

    virtual void AddTags() override;
} GNativeTags;
