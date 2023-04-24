// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TaggedTypes/PGTaggedInputAction.h"
#include "PGInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class PLANETGUARDIAN_API UPGInputConfig : public UDataAsset
{
	GENERATED_BODY()

	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="Input Action", AllowPrivateAccess="true"))
	TSet<FPGTaggedInputAction> InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 InputMappingContextPriority;

public:
	// Returns the first Input Action associated with a given tag.
	const UInputAction* FindInputActionForTag(const FGameplayTag& Tag) const;

	const UInputMappingContext* GetInputMappingContext() const { return InputMappingContext; }

	int32 GetInputMappingContextPriority() const { return InputMappingContextPriority; }
};
