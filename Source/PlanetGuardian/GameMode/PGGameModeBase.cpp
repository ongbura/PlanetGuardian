// Copyright Epic Games, Inc. All Rights Reserved.


#include "PGGameModeBase.h"

#include "System/PGSingleton.h"

void APGGameModeBase::InitGameState()
{
	Super::InitGameState();

	UPGSingleton::Get().SetCurrentWorld(GetWorld());
}
