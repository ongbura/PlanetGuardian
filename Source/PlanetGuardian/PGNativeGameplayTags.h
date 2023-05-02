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
	
	// Movement.Mode
	FGameplayTag Movement_Mode_Falling{};
	FGameplayTag Movement_Mode_Flying{};
	FGameplayTag Movement_Mode_NavWalking{};
	FGameplayTag Movement_Mode_Walking{};

    virtual void AddTags() override;
} GNativeTags;
