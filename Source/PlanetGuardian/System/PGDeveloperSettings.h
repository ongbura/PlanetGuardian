// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PGDeveloperSettings.generated.h"

#define DEFAULT_NEW_EFFECT_EMITTERS (5)

class UPGInputActionData;
class UPGEffectSetData;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Planet Guardian Settings"))
class PLANETGUARDIAN_API UPGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input Configuration", meta=(DisplayName="Input Action Configuration Data"))
	TSoftObjectPtr<UPGInputActionData> InputActionConfigData;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration")
	int32 InitialEffectEmitterPoolSize { 50 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration")
	bool bAddEffectEmittersWhenRunOutOfPool { true };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration", meta=(ClampMin="1", ClampMax="100", EditCondition="bAddEffectEmittersWhenRunOutOfPool", EditConditionHides))
	int32 NumNewEffectEmittersPerFrame { DEFAULT_NEW_EFFECT_EMITTERS };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration", meta=(RelativeToGameContentDir, LongPackageName))
	FDirectoryPath EffectSetDataPath;
	
	UPGDeveloperSettings() = default;

protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
