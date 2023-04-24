// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "NativeGameplayTags.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    // Attribute.Common
	Attribute_Common_Health = GameplayTagsManager.AddNativeGameplayTag("Attribute.Common.Health");
	
	// Input.Action
	Input_Action_Jump = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Jump");
	Input_Action_Look = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Look");
	Input_Action_Move = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Move");

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
