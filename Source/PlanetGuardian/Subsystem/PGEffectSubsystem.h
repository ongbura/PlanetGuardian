// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameInstanceSubsystem.h"
#include "PGEffectSubsystem.generated.h"

class APGEffectorEmitter;
class UPGEffectSetData;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class PLANETGUARDIAN_API UPGEffectSubsystem : public UPGGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FSoftObjectPath, const UPGEffectSetData*> EffectSetMap;

	UPROPERTY()
	TArray<APGEffectorEmitter*> AllEffectEmitters;

	TQueue<APGEffectorEmitter*, EQueueMode::Mpsc> EmitterPool;
	int32 EmitterPoolSize { 0 };

	TSet<APGEffectorEmitter*> ActivatedEmitters;

	mutable FCriticalSection ActivatedEmittersMutex;

public:
	static UPGEffectSubsystem* Get();
	
	APGEffectorEmitter* PopEffectEmitter(const TSoftObjectPtr<UPGEffectSetData>& SoftEffectSet);

	void PushEffectEmitter(APGEffectorEmitter* Emitter);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void GenerateEffectEmitters(int32 NumEmitters);

	const UPGEffectSetData* FindOrLoadEffectSet(const TSoftObjectPtr<UPGEffectSetData>& SoftEffectSet);

	UFUNCTION()
	void OnInitialEffectsLoaded();
};
