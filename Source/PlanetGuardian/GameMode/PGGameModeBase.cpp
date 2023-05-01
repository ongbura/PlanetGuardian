// Copyright Epic Games, Inc. All Rights Reserved.


#include "PGGameModeBase.h"

#include "System/PGGameGlobals.h"

void APGGameModeBase::InitGameState()
{
	Super::InitGameState();

	UPGGameGlobals::Get().SetCurrentWorld(GetWorld());
}
