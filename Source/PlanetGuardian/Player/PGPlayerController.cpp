// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayerController.h"
#include "Character/PGCharacter.h"

void APGPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	
	const auto* Avatar = CastChecked<APGCharacter>(P);
	Avatar->OnControllerChanged.Broadcast();
}
