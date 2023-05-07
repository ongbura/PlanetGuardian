// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PGDeveloperSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Planet Guardian Settings"))
class PLANETGUARDIAN_API UPGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 EffectEmitterPoolCapacity { 256 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 NumInitialEffectEmitters { 64 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 NumNewEffectEmittersPerFrame { 8 };
	
	UPGDeveloperSettings() = default;
};
