// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PGGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	template <typename T>
	T* Get(const UObject* WorldContextObject);
};

template <typename T>
T* UPGGameInstanceSubsystem::Get(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
	{
		return nullptr;
	}

	const auto* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return nullptr;
	}

	return UGameInstance::GetSubsystem<T>(World->GetGameInstance());
}
