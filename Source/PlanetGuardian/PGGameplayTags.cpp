// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGameplayTags.h"
#include "GameplayTagsManager.h"

void FPGInputTags::AddTags()
{
	auto& GameplayTagsManager = UGameplayTagsManager::Get();

	Input_Action_Move = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Move");
	Input_Action_Look = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Look");
	Input_Action_Jump = GameplayTagsManager.AddNativeGameplayTag("Input.Action.Jump");

	GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
