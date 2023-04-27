// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PGTaggedInputAction.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FPGTaggedInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UInputAction> InputAction { nullptr };

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Input.Action"))
	FGameplayTag Tag;

	FORCEINLINE friend uint32 GetTypeHash(const FPGTaggedInputAction& TaggedInputAction)
	{
		return ::GetTypeHash(TaggedInputAction.Tag.GetTagName());
	}

	bool operator==(const FPGTaggedInputAction& Other) const
	{
		return Tag == Other.Tag;
	}

	bool Equals(const FPGTaggedInputAction& Other) const
	{
		return Tag == Other.Tag;
	}
};
