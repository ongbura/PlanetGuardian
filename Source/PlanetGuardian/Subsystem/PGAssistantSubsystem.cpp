// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAssistantSubsystem.h"
#include "System/PGSingleton.h"
#include "InputAction.h"
#include "System/PGDeveloperSettings.h"
#include "AbilitySystem/PGGameplayAbility.h"

#define DEFAULT_ABILITY_LEVEL (1)

UPGAssistantSubsystem* UPGAssistantSubsystem::Get()
{
	return UPGSingleton::Get().GetGameInstanceSubsystem<UPGAssistantSubsystem>();
}

const UInputAction* UPGAssistantSubsystem::FindInputAction(const FGameplayTag& InputActionTag) const
{
	auto* Ret = InputActions.Find(InputActionTag);
	return Ret != nullptr ? *Ret : nullptr;
}

bool UPGAssistantSubsystem::FindGameplayAbilities(const UInputAction* InputAction,
                                                  TArray<const UPGGameplayAbility*>& OutAbilities) const
{
	if (InputAction == nullptr)
	{
		return false;
	}
	
	if (auto* Abilities = InputActionAbilities.Find(InputAction))
	{
		OutAbilities.Empty((*Abilities).Num());
		for (const auto* Ability : *Abilities)
		{
			OutAbilities.Add(Ability);
		}

		return true;
	}

	return false;
}

void UPGAssistantSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const auto* Settings = GetDefault<UPGDeveloperSettings>())
	{
		if (const auto* InputActionConfig = Settings->InputActionConfigData.LoadSynchronous())
		{
			for (const auto& InputActionInfo : InputActionConfig->GetInputActionInfos())
			{
				auto* LoadedInputAction = InputActionInfo.LoadInputAction();
				if (LoadedInputAction == nullptr)
				{
					continue;
				}

				auto LoadedAbilities = InputActionInfo.LoadGameplayAbilities();

				InputActions.Add(InputActionInfo.InputActionTag, LoadedInputAction);
				InputActionAbilities.Add(LoadedInputAction, MoveTemp(LoadedAbilities));
			}
		}
	}
}
