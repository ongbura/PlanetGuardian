// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "PGSingleton.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGSingleton : public UObject
{
	GENERATED_BODY()

	TWeakObjectPtr<UWorld> CurrentWorld;

public:
	static UPGSingleton& Get()
	{
		return *Cast<UPGSingleton>(GEngine->GameSingleton);
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
