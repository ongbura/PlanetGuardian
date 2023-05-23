// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace PGGameplayTags
{
    // Ability.ActivateFail
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	
	// Ability.Guardian
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Guardian_Movement_Crouch);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Guardian_Movement_Jetpack);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Guardian_Movement_Jump);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Guardian_Movement_Sprint);
	
	// Attribute.Common
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Common_Health);
	
	// Attribute.Guardian
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Guardian_JetpackPower);
	
	// State.Guardian
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Guardian_IsFiring);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Guardian_IsOnADS);
	PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Guardian_IsReloading);
}
