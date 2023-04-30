// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGEffectSetData.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class PLANETGUARDIAN_API UPGEffectSetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraSystem> VisualFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USoundBase> SoundFX;

public:
	UNiagaraSystem* GetVisualFX() const { return VisualFX; }

	USoundBase* GetSoundFX() const { return SoundFX; }

	static FPrimaryAssetType GetPrimaryAssetType() { return FPrimaryAssetType("EffectSet"); }

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
