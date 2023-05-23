// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectSubsystem.h"
#include "System/PGGameGlobals.h"
#include "NiagaraSystem.h"
#include "PGEffectBundle.h"
#include "PGLogChannels.h"
#include "System/PGAssetManager.h"
#include "System/PGDeveloperSettings.h"
#include "Sound/SoundBase.h"

UNiagaraSystem* UPGEffectSubsystem::GetVisualFX(const FSoftObjectPath& SoftVisualFXPath)
{
	if (!SoftVisualFXPath.IsValid())
	{
		return nullptr;
	}
	
	const auto* PVisualFX = VisualFXMap.Find(SoftVisualFXPath);
	if (PVisualFX == nullptr)
	{
		return ForceLoadVisualFX(SoftVisualFXPath);
	}

	return *PVisualFX;
}

USoundBase* UPGEffectSubsystem::GetSoundFX(const FSoftObjectPath& SoftSoundFXPath)
{
	if (!SoftSoundFXPath.IsValid())
	{
		return nullptr;
	}
	
	const auto* PSoundFX = SoundFXMap.Find(SoftSoundFXPath);
	if (PSoundFX == nullptr)
	{
		return ForceLoadSoundFX(SoftSoundFXPath);
	}

	return *PSoundFX;
}

void UPGEffectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	auto& AssetManager = UPGAssetManager::Get();

	TArray<FPrimaryAssetId> EffectSetPrimaryAssetIds;	
	AssetManager.GetPrimaryAssetIdList(UPGAssetManager::EffectBundleType, EffectSetPrimaryAssetIds);

	auto Delegate = FStreamableDelegate::CreateUObject(this, &UPGEffectSubsystem::OnEffectBundlesLoaded, MoveTemp(EffectSetPrimaryAssetIds));
	AssetManager.LoadPrimaryAssets(EffectSetPrimaryAssetIds, {}, MoveTemp(Delegate));
}

bool UPGEffectSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const auto* World = Cast<UWorld>(Outer);
	check(World);
	
	return World->GetAuthGameMode() == nullptr;
}

void UPGEffectSubsystem::OnEffectBundlesLoaded(TArray<FPrimaryAssetId> LoadedAssetIds)
{
	const auto& AssetManager = UPGAssetManager::Get();

	for (const auto& AssetId : LoadedAssetIds)
	{
		const auto* AssetObject = AssetManager.GetPrimaryAssetObject(AssetId);
		if (AssetObject == nullptr)
		{
			continue;
		}
		
		const auto* EffectBundle = CastChecked<UPGEffectBundle>(AssetObject);
		
		const auto VisualFXPath = EffectBundle->GetSoftVisualFXPath();
		if (VisualFXPath.IsValid())
		{
			auto* VisualFX = EffectBundle->GetVisualFX();
			check(VisualFX);

			VisualFXMap.Add(EffectBundle->GetSoftVisualFXPath(), VisualFX);
		}

		const auto SoundFXPath = EffectBundle->GetSoftSoundFXPath();
		if (SoundFXPath.IsValid())
		{
			auto* SoundFX = EffectBundle->GetSoundFX();
			check(SoundFX);

			SoundFXMap.Add(EffectBundle->GetSoftSoundFXPath(), SoundFX);
		}
	}
}

UNiagaraSystem* UPGEffectSubsystem::ForceLoadVisualFX(const FSoftObjectPath& SoftVisualFXPath)
{
	auto* LoadedObject = SoftVisualFXPath.TryLoad();
	check(LoadedObject);

	auto* VisualFX = CastChecked<UNiagaraSystem>(LoadedObject);
	return VisualFXMap.Add(SoftVisualFXPath, VisualFX);
}

USoundBase* UPGEffectSubsystem::ForceLoadSoundFX(const FSoftObjectPath& SoftSoundFXPath)
{
	auto* LoadedObject = SoftSoundFXPath.TryLoad();
	check(LoadedObject);

	auto* SoundFX = CastChecked<USoundBase>(LoadedObject);
	return SoundFXMap.Add(SoftSoundFXPath, SoundFX);
}