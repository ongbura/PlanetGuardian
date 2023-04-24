// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGPlayerController.h"
#include "InputActionValue.h"
#include "PGGuardianController.generated.h"

class UPGInputConfig;

UCLASS()
class PLANETGUARDIAN_API APGGuardianController : public APGPlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPGInputConfig> InputConfig;

public:
	APGGuardianController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJumping(const FInputActionValue& Value);
};
