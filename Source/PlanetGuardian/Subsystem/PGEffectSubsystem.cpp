// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectSubsystem.h"
#include "System/PGGameGlobals.h"
#include "NiagaraSystem.h"
#include "System/PGAssetManager.h"
#include "System/PGProjectSettings.h"
#include "Sound/SoundBase.h"

UPGEffectSubsystem* UPGEffectSubsystem::Get()
{
	return UPGGameGlobals::Get().GetGameInstanceSubsystem<UPGEffectSubsystem>();
}

UNiagaraSystem* UPGEffectSubsystem::FindOrLoadNiagaraSystem(const TSoftObjectPtr<UNiagaraSystem>& SoftNiagaraSystem)
{
	const auto* NiagaraSystem = VisualFXMap.Find(SoftNiagaraSystem.ToSoftObjectPath());
	if (NiagaraSystem != nullptr)
	{
		return *NiagaraSystem;
	}

	auto* LoadedNiagaraSystem = SoftNiagaraSystem.LoadSynchronous();
	VisualFXMap.Add(SoftNiagaraSystem.ToSoftObjectPath(), LoadedNiagaraSystem);

	return LoadedNiagaraSystem;
}

USoundBase* UPGEffectSubsystem::FindOrLoadSoundBase(const TSoftObjectPtr<USoundBase>& SoftSoundBase)
{
	const auto* SoundBase = SoundFXMap.Find(SoftSoundBase.ToSoftObjectPath());
	if (SoundBase != nullptr)
	{
		return *SoundBase;
	}

	auto* LoadedSoundBase = SoftSoundBase.LoadSynchronous();
	SoundFXMap.Add(SoftSoundBase.ToSoftObjectPath(), LoadedSoundBase);

	return LoadedSoundBase;
}

void UPGEffectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AsyncLoadVisualSoundEffects();
}

void UPGEffectSubsystem::AsyncLoadVisualSoundEffects()
{
	const auto* Settings = GetDefault<UPGProjectSettings>();
	if (Settings == nullptr)
	{
		return;
	}

	const auto& AssetRegistry = UPGAssetManager::Get().GetAssetRegistry();

	FTopLevelAssetPath NiagaraAssetPath { TEXT("/Script/Niagara"), TEXT("NiagaraSystem") };
	FTopLevelAssetPath SoundAssetPath { TEXT("/Script/Engine"), TEXT("SoundBase") };

	FARFilter Filter;
	Filter.ClassPaths.AddUnique(MoveTemp(NiagaraAssetPath));
	Filter.ClassPaths.AddUnique(MoveTemp(SoundAssetPath));
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(*Settings->PlayEffectPath.Path);

	TArray<FAssetData> StreamableEffectData;

	AssetRegistry.GetAssets(Filter, StreamableEffectData);
	TArray<FSoftObjectPath> EffectsToStream;

	for (const auto& Data : StreamableEffectData)
	{
		EffectsToStream.AddUnique(Data.GetSoftObjectPath());
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	auto Delegate = FStreamableDelegate::CreateUObject(this, &UPGEffectSubsystem::OnAsyncLoadVisualSoundEffectsComplete,
													   MoveTemp(StreamableEffectData));

	Streamable.RequestAsyncLoad(MoveTemp(EffectsToStream), MoveTemp(Delegate));
}

void UPGEffectSubsystem::OnAsyncLoadVisualSoundEffectsComplete(TArray<FAssetData> LoadedEffectData)
{
	TMap<FSoftObjectPath, UNiagaraSystem*> LoadedNiagaraSystems;
	TMap<FSoftObjectPath, USoundBase*> LoadedSounds;

	for (const auto& Data : LoadedEffectData)
	{
		auto* Effect = Data.GetAsset();
		check(Effect != nullptr);

		if (Effect->IsA(USoundBase::StaticClass()))
		{
			if (auto* SoundBase = Cast<USoundBase>(Effect))
			{
				LoadedSounds.Add(Data.GetSoftObjectPath(), SoundBase);
			}
		}
		else if (Effect->IsA(UNiagaraSystem::StaticClass()))
		{
			if (auto* NiagaraSystem = Cast<UNiagaraSystem>(Effect))
			{
				LoadedNiagaraSystems.Add(Data.GetSoftObjectPath(), NiagaraSystem);
			}
		}
		else
		{
			checkNoEntry();
		}
	}

	auto* EffectSubsystem = UPGEffectSubsystem::Get();
	check(EffectSubsystem);

	VisualFXMap.Append(MoveTemp(LoadedNiagaraSystems));
	SoundFXMap.Append(MoveTemp(LoadedSounds));
}
