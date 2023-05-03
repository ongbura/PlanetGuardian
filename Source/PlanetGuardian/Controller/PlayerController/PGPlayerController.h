// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

class UPGAbilitySystemComponent;
class UPGInputConfig;
class UPGHUD;

UCLASS()
class PLANETGUARDIAN_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UPGHUD> HUDClass;

	UPROPERTY()
	TObjectPtr<UPGHUD> HUD;

public:
	void CreateHUD(UPGAbilitySystemComponent* ASC);

	void DestroyHUD();

	UPGHUD* GetPGHUD() const { return HUD; }
	
protected:
	virtual void AcknowledgePossession(APawn* P) override;

};
