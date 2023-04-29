// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PGDeveloperSettings.generated.h"

class UPGInputActionData;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Planet Guardian Settings"))
class PLANETGUARDIAN_API UPGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input Configuration", meta=(DisplayName="Input Action Configuration Data"))
	TSoftObjectPtr<UPGInputActionData> InputActionConfigData;
	
	UPGDeveloperSettings() = default;
};
