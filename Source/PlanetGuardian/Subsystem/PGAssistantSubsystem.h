// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PGGameInstanceSubsystem.h"
#include "DataAsset/PGInputActionData.h"
#include "PGAssistantSubsystem.generated.h"

class UInputAction;
class UPGGameplayAbility;

UCLASS()
class PLANETGUARDIAN_API UPGAssistantSubsystem : public UPGGameInstanceSubsystem
{
	GENERATED_BODY()

	TMap<FGameplayTag, UInputAction*> InputActions;

	TMap<UInputAction*, TArray<const UPGGameplayAbility*>> InputActionAbilities;
	
public:
	static UPGAssistantSubsystem* Get();

	const UInputAction* FindInputAction(const FGameplayTag& InputActionTag) const;

	bool FindGameplayAbilities(const UInputAction* InputAction, TArray<const UPGGameplayAbility*>& OutAbilities) const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
