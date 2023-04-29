// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputActionData.h"
#include "InputAction.h"
#include "AbilitySystem/PGGameplayAbility.h"

UInputAction* FPGInputActionInfo::LoadInputAction() const
{
	return InputAction.LoadSynchronous();
}

TArray<const UPGGameplayAbility*> FPGInputActionInfo::LoadGameplayAbilities() const
{
	TArray<const UPGGameplayAbility*> LoadedAbilities;
	for (const TSoftClassPtr<UPGGameplayAbility>& AbilityClass : AbilityClasses)
	{
		if (AbilityClass.IsValid())
		{
			const auto* LoadedAbility = AbilityClass.LoadSynchronous()->GetDefaultObject<UPGGameplayAbility>();
			LoadedAbilities.Add(LoadedAbility);
		}
	}

	return LoadedAbilities;
}
