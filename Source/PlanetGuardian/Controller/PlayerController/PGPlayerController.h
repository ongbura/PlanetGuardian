// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

UCLASS()
class PLANETGUARDIAN_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(APGPlayerController, FOnAcknowledgePossession, APawn*)
	
public:
	FOnAcknowledgePossession OnAcknowledgePossession;

protected:
	virtual void AcknowledgePossession(APawn* P) override;
};
