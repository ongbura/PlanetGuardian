// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAssetManager.h"

UPGAssetManager& UPGAssetManager::Get()
{
	static auto* const AssetManager = Cast<UPGAssetManager>(&UAssetManager::Get());
	return *AssetManager;
}

void UPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// UPGGameplayTagsManager::Get().InitializePlanetGuardianTags();
}
