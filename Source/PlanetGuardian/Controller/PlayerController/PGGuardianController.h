// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGPlayerController.h"
#include "PGGuardianController.generated.h"

class UPGAbilitySystemComponent;
class UPGHUD;

UCLASS()
class PLANETGUARDIAN_API APGGuardianController : public APGPlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UPGHUD> HUDClass;

	UPROPERTY()
	TObjectPtr<UPGHUD> HUD;

public:
	APGGuardianController();
	
	void MakeHUDVisible(const UPGAbilitySystemComponent* ASC);

	void MakeHUDInvisible() const;

	UPGHUD* GetPGHUD() const { return HUD; }

protected:
	virtual void AcknowledgePossession(APawn* P) override;

private:
	void CreateHUD();
};
