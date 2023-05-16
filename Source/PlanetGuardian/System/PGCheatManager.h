// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PGCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec, Category="Debug")
	void PGDebug(FName DebugType) const;
};
