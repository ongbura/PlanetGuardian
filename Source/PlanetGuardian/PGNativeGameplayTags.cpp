// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"

namespace PGGameplayTags
{
    // Ability.ActivateFail
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	
	// Ability.Guardian
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guardian_Movement_Crouch, "Ability.Guardian.Movement.Crouch", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guardian_Movement_Jetpack, "Ability.Guardian.Movement.Jetpack", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guardian_Movement_Jump, "Ability.Guardian.Movement.Jump", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guardian_Movement_Sprint, "Ability.Guardian.Movement.Sprint", "");
	
	// Attribute.Common
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Common_Health, "Attribute.Common.Health", "");
	
	// Attribute.Guardian
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Guardian_JetpackPower, "Attribute.Guardian.JetpackPower", "");
	
	// State.Guardian
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Guardian_IsFiring, "State.Guardian.IsFiring", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Guardian_IsOnADS, "State.Guardian.IsOnADS", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Guardian_IsReloading, "State.Guardian.IsReloading", "");
}
