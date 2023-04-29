// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PGInputActionData.generated.h"

class UInputAction;
class UPGGameplayAbility;

USTRUCT()
struct FPGInputActionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Input.Action"))
	FGameplayTag InputActionTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<UPGGameplayAbility>> AbilityClasses;

	FORCEINLINE friend uint32 GetTypeHash(const FPGInputActionInfo& Data)
	{
		return GetTypeHash(Data.InputActionTag);
	}

	bool operator==(const FPGInputActionInfo& Other) const
	{
		return InputActionTag == Other.InputActionTag && InputAction == Other.InputAction && AbilityClasses == Other.AbilityClasses;
	}

	bool Equals(const FPGInputActionInfo& Other) const
	{
		return InputActionTag == Other.InputActionTag && InputAction == Other.InputAction && AbilityClasses == Other.AbilityClasses;
	}

	UInputAction* LoadInputAction() const;

	TArray<const UPGGameplayAbility*> LoadGameplayAbilities() const;
};

UCLASS()
class PLANETGUARDIAN_API UPGInputActionData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSet<FPGInputActionInfo> InputActionInfos;
	
public:
	const auto& GetInputActionInfos() const { return InputActionInfos; }
};
