// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "NativeGameplayTags.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    // Ability.Crouch
	Ability_Crouch = GameplayTagsManager.AddNativeGameplayTag("Ability.Crouch");
	
	// Ability.Jump
	Ability_Jump = GameplayTagsManager.AddNativeGameplayTag("Ability.Jump");
	
	// Attribute.Common
	Attribute_Common_Health = GameplayTagsManager.AddNativeGameplayTag("Attribute.Common.Health");
	
	// Movement.Mode
	Movement_Mode_Falling = GameplayTagsManager.AddNativeGameplayTag("Movement.Mode.Falling");
	Movement_Mode_Flying = GameplayTagsManager.AddNativeGameplayTag("Movement.Mode.Flying");
	Movement_Mode_NavWalking = GameplayTagsManager.AddNativeGameplayTag("Movement.Mode.NavWalking");
	Movement_Mode_Walking = GameplayTagsManager.AddNativeGameplayTag("Movement.Mode.Walking");

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
