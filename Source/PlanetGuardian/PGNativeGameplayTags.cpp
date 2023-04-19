// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "GameplayTagsManager.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    Input_Action_Jump = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Jump");
	Input_Action_Look = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Look");
	Input_Action_Move = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Move");

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
