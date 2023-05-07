// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAssetManager.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType UPGAssetManager::EffectBundleType = TEXT("EffectBundle");

UPGAssetManager& UPGAssetManager::Get()
{
	static auto* const AssetManager = Cast<UPGAssetManager>(&UAssetManager::Get());
	return *AssetManager;
}

void UPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
