// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectSubsystem.h"
#include "Cosmetics/PGEffectorEmitter.h"
#include "DataAsset/PGEffectSetData.h"
#include "System/PGDeveloperSettings.h"
#include "System/PGAssetManager.h"
#include "System/PGSingleton.h"

UPGEffectSubsystem* UPGEffectSubsystem::Get()
{
	return UPGSingleton::Get().GetGameInstanceSubsystem<UPGEffectSubsystem>();
}

APGEffectorEmitter* UPGEffectSubsystem::PopEffectEmitter(const TSoftObjectPtr<UPGEffectSetData>& SoftEffectSet)
{
	APGEffectorEmitter* Emitter = nullptr;
	if (EmitterPool.Dequeue(Emitter))
	{
		check(Emitter);

		const auto* EffectSet = FindOrLoadEffectSet(SoftEffectSet);
		check(EffectSet);

		Emitter->SetVisualEffect(EffectSet->GetVisualFX());
		Emitter->SetSoundEffect(EffectSet->GetSoundFX());

		{
			FScopeLock Lock(&ActivatedEmittersMutex);

			check(ActivatedEmitters.Find(Emitter) == nullptr);
			ActivatedEmitters.Add(Emitter);
		}
	}
	else
	{
		const auto* Settings = GetDefault<UPGDeveloperSettings>();

		if (Settings != nullptr && Settings->bAddEffectEmittersWhenRunOutOfPool)
		{
			static int32 NumNewEmitters = Settings->NumNewEffectEmittersPerFrame;
			GenerateEffectEmitters(NumNewEmitters);
		}
	}

	return Emitter;
}

void UPGEffectSubsystem::PushEffectEmitter(APGEffectorEmitter* Emitter)
{
	{
		FScopeLock Lock(&ActivatedEmittersMutex);

		// check(ActivatedEmitters.Find(Emitter) != nullptr);
		ActivatedEmitters.Remove(Emitter);
	}

	verify(EmitterPool.Enqueue(Emitter));
}

void UPGEffectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const auto* Settings = GetDefault<UPGDeveloperSettings>())
	{
		GenerateEffectEmitters(Settings->InitialEffectEmitterPoolSize);

		const auto& AssetRegistry = UPGAssetManager::Get().GetAssetRegistry();

		FARFilter Filter;
		Filter.ClassPaths.AddUnique({TEXT("/Script/PlanetGuardian"), TEXT("PGEffectSetData")});
		Filter.bRecursivePaths = true;
		Filter.PackagePaths.Add(*Settings->EffectSetDataPath.Path);

		TArray<FAssetData> EffectSetData;

		AssetRegistry.GetAssets(Filter, EffectSetData);
		TArray<FSoftObjectPath> EffectsToStream;

		for (const auto& Data : EffectSetData)
		{
			EffectsToStream.AddUnique(Data.GetSoftObjectPath());
		}

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		auto Delegate = FStreamableDelegate::CreateUObject(this, &UPGEffectSubsystem::OnInitialEffectsLoaded);

		Streamable.RequestAsyncLoad(MoveTemp(EffectsToStream), MoveTemp(Delegate));
	}
}

void UPGEffectSubsystem::GenerateEffectEmitters(const int32 NumEmitters)
{
	for (int32 i = 0; i < NumEmitters; ++i)
	{
		APGEffectorEmitter* Emitter = Cast<APGEffectorEmitter>(
			GetWorld()->SpawnActor(APGEffectorEmitter::StaticClass()));
		check(Emitter);

		verify(EmitterPool.Enqueue(Emitter));
		AllEffectEmitters.Add(Emitter);
	}

	EmitterPoolSize += NumEmitters;
}

const UPGEffectSetData* UPGEffectSubsystem::FindOrLoadEffectSet(const TSoftObjectPtr<UPGEffectSetData>& SoftEffectSet)
{
	if (const auto* Found = EffectSetMap.Find(SoftEffectSet.Get()))
	{
		return *Found;
	}

	const auto* LoadedEffectSet = SoftEffectSet.LoadSynchronous();
	EffectSetMap.Add(SoftEffectSet.ToSoftObjectPath(), LoadedEffectSet);

	return LoadedEffectSet;
}

void UPGEffectSubsystem::OnInitialEffectsLoaded()
{
	const auto& AssetManager = UAssetManager::Get();

	TArray<UObject*> LoadedEffects;
	AssetManager.GetPrimaryAssetObjectList(UPGEffectSetData::GetPrimaryAssetType(), LoadedEffects);

	for (const auto* LoadedEffect : LoadedEffects)
	{
		const auto* EffectSetData = Cast<UPGEffectSetData>(LoadedEffect);
		check(EffectSetData);

		const auto SoftPath = AssetManager.GetPrimaryAssetPath(EffectSetData->GetPrimaryAssetId());

		EffectSetMap.Add(SoftPath, EffectSetData);
	}
}
