// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputActionAbilitySet.h"
#include "InputAction.h"
#include "AbilitySystem/PGGameplayAbility.h"

UInputAction* FPGInputActionAbilitySet::LoadInputAction() const
{
	return InputAction.LoadSynchronous();
}

UPGGameplayAbility* FPGInputActionAbilitySet::LoadGameplayAbility() const
{
	return AbilityClass.LoadSynchronous()->GetDefaultObject<UPGGameplayAbility>();
}
