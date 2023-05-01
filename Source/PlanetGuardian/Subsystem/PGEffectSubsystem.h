// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameInstanceSubsystem.h"
#include "PGEffectSubsystem.generated.h"

class APGEffectEmitter;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class PLANETGUARDIAN_API UPGEffectSubsystem : public UPGGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FSoftObjectPath, UNiagaraSystem*> VisualFXMap;

	UPROPERTY()
	TMap<FSoftObjectPath, USoundBase*> SoundFXMap;

public:
	static UPGEffectSubsystem* Get();

	UNiagaraSystem* FindOrLoadNiagaraSystem(const TSoftObjectPtr<UNiagaraSystem>& SoftNiagaraSystem);

	USoundBase* FindOrLoadSoundBase(const TSoftObjectPtr<USoundBase>& SoftSoundBase);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

private:
	void AsyncLoadVisualSoundEffects();

	UFUNCTION()
	void OnAsyncLoadVisualSoundEffectsComplete(TArray<FAssetData> LoadedEffectData);
};
