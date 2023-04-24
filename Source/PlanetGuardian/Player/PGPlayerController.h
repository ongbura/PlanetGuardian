// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

#define RETURN_PAWN_IF_NULL() if (GetPawn() == nullptr) { return; }

UCLASS()
class PLANETGUARDIAN_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void AcknowledgePossession(APawn* P) override;

};
