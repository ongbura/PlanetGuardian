// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PGAssetManager.generated.h"

/**
 *
 */
UCLASS()
class PLANETGUARDIAN_API UPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UPGAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
