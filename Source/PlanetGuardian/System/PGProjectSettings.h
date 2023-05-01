// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PGProjectSettings.generated.h"

#define DEFAULT_NEW_EFFECT_EMITTERS (16)

class UPGInputActionAbilityData;
class UPGEffectSetData;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Planet Guardian Settings"))
class PLANETGUARDIAN_API UPGProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input Configuration", meta=(DisplayName="Input Action Ability Data"))
	TSoftObjectPtr<UPGInputActionAbilityData> InputActionAbilityData;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect Emitter Pool")
	int32 EffectEmitterPoolCapacity { 256 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect Emitter Pool")
	int32 NumInitialEffectEmitters { 64 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect Emitter Pool")
	int32 NumNewEffectEmittersPerFrame { DEFAULT_NEW_EFFECT_EMITTERS };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect", meta=(RelativeToGameContentDir, LongPackageName))
	FDirectoryPath PlayEffectPath;

	UPGProjectSettings() = default;
};
