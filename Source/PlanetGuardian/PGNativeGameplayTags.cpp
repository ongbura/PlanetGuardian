// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "NativeGameplayTags.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    // Ability.Guardian
	Ability_Guardian_Movement_Jump = GameplayTagsManager.AddNativeGameplayTag("Ability.Guardian.Movement.Jump");
	Ability_Guardian_Movement_Sprint = GameplayTagsManager.AddNativeGameplayTag("Ability.Guardian.Movement.Sprint");
	
	// Attribute.Common
	Attribute_Common_Health = GameplayTagsManager.AddNativeGameplayTag("Attribute.Common.Health");
	
	// Attribute.Guardian
	Attribute_Guardian_JetpackPower = GameplayTagsManager.AddNativeGameplayTag("Attribute.Guardian.JetpackPower");

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
