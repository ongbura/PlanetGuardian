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

	const UWorld* GetCurrentWorld() const { return CurrentWorld.IsValid() ? CurrentWorld.Get() : nullptr; }

	template <typename FmtType, typename... Types>
	void PrintMessage(FColor Color, const FmtType& Fmt, Types&&... Args) const;
};

template <typename FmtType, typename ... Types>
void UPGGameGlobals::PrintMessage(const FColor Color, const FmtType& Fmt, Types&&... Args) const
{
	const FString Message = FString::Printf(Fmt, Forward<Types>(Args)...);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, Color, Message);
}
