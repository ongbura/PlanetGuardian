// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameGlobals.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGGameGlobals : public UObject
{
	GENERATED_BODY()

	TWeakObjectPtr<UWorld> CurrentWorld;

public:
	static UPGGameGlobals& Get()
	{
		return *Cast<UPGGameGlobals>(GEngine->GameSingleton);
	}
	
	template <typename T>
	T* GetGameInstanceSubsystem()
	{
		if (CurrentWorld.IsValid())
		{
			return CurrentWorld->GetGameInstance()->GetSubsystem<T>();
		}
		
		return nullptr;
	}

	void SetCurrentWorld(UWorld* World) { CurrentWorld.Reset(); CurrentWorld = World; }
};
