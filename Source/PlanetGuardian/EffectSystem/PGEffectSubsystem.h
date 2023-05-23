// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/PGWorldSubsystem.h"
#include "PGEffectSubsystem.generated.h"

class UPGEffectBundle;
class APGEffectEmitter;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class PLANETGUARDIAN_API UPGEffectSubsystem : public UPGWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FSoftObjectPath, UNiagaraSystem*> VisualFXMap;

	UPROPERTY()
	TMap<FSoftObjectPath, USoundBase*> SoundFXMap;

public:
	UNiagaraSystem* GetVisualFX(const FSoftObjectPath& SoftVisualFXPath);

	USoundBase* GetSoundFX(const FSoftObjectPath& SoftSoundFXPath);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	void OnEffectBundlesLoaded(TArray<FPrimaryAssetId> LoadedAssetIds);	

	UNiagaraSystem* ForceLoadVisualFX(const FSoftObjectPath& SoftVisualFXPath);

	USoundBase* ForceLoadSoundFX(const FSoftObjectPath& SoftSoundFXPath);
};
