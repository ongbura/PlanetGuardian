// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "NativeGameplayTags.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    // Ability.Guardian
	Ability_Guardian_Movement_Crouch = GameplayTagsManager.AddNativeGameplayTag("Ability.Guardian.Movement.Crouch");
	Ability_Guardian_Movement_Jump = GameplayTagsManager.AddNativeGameplayTag("Ability.Guardian.Movement.Jump");
	Ability_Guardian_Movement_Sprint = GameplayTagsManager.AddNativeGameplayTag("Ability.Guardian.Movement.Sprint");
	
	// Attribute.Common
	Attribute_Common_Health = GameplayTagsManager.AddNativeGameplayTag("Attribute.Common.Health");
	
	// Attribute.Guardian
	Attribute_Guardian_JetpackPower = GameplayTagsManager.AddNativeGameplayTag("Attribute.Guardian.JetpackPower");
	
	// State.Guardian
	State_Guardian_IsFiring = GameplayTagsManager.AddNativeGameplayTag("State.Guardian.IsFiring");
	State_Guardian_IsOnADS = GameplayTagsManager.AddNativeGameplayTag("State.Guardian.IsOnADS");
	State_Guardian_IsReloading = GameplayTagsManager.AddNativeGameplayTag("State.Guardian.IsReloading");

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
