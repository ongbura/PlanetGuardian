// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGEffectBundle.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class PLANETGUARDIAN_API UPGEffectBundle : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UNiagaraSystem> SoftVisualFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<USoundBase> SoftSoundFX;

public:
	FSoftObjectPath GetSoftVisualFXPath() const;
	
	UNiagaraSystem* GetVisualFX() const;

	FSoftObjectPath GetSoftSoundFXPath() const;
	
	USoundBase* GetSoundFX() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
