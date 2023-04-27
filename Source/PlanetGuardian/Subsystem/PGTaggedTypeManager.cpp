// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGTaggedTypeManager.h"
#include "DataAsset/PGTaggedData.h"
#include "System/PGSingleton.h"
#include "InputAction.h"
#include "System/PGDeveloperSettings.h"
#include "AbilitySystem/PGGameplayAbility.h"

UPGTaggedTypeManager* UPGTaggedTypeManager::Get()
{
	return UPGSingleton::Get().GetGameInstanceSubsystem<UPGTaggedTypeManager>();
}

UInputAction* UPGTaggedTypeManager::FindInputAction(const FGameplayTag& Tag) const
{
	auto* Data = InputActions.Find(Tag);
	return Data != nullptr ? *Data : nullptr;
}

UPGGameplayAbility* UPGTaggedTypeManager::FindAbility(const FGameplayTag& Tag) const
{
	auto* Data = Abilities.Find(Tag);
	return Data != nullptr ? *Data : nullptr;
}

void UPGTaggedTypeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const auto* Settings = GetDefault<UPGDeveloperSettings>())
	{
		if (const auto* InputActionConfig = Settings->InputActions.LoadSynchronous())
		{
			for (const auto& [Tag, InputAction] : InputActionConfig->GetInputActions())
			{
				InputActions.Add(Tag, InputAction.LoadSynchronous());
			}
		}

		if (const auto* AbilityConfig = Settings->Abilities.LoadSynchronous())
		{
			for (const auto& [Tag, Ability] : AbilityConfig->GetAbilities())
			{
				Abilities.Add(Tag, Ability.LoadSynchronous());
			}
		}
	}
}
