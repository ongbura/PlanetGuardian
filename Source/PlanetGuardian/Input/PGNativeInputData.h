// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGNativeInputData.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PLANETGUARDIAN_API UPGNativeInputData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UInputMappingContext> NativeInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 NativeInputMappingContextPriority { 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UInputAction> LookInputAction;

public:
	const auto& GetSoftNativeInputMappingContext() const { return NativeInputMappingContext; }
	int32 GetNativeInputMappingContextPriority() const { return NativeInputMappingContextPriority; }
	const auto& GetSoftMoveInputAction() const { return MoveInputAction; }
	const auto& GetSoftLookInputAction() const { return LookInputAction; }
};
