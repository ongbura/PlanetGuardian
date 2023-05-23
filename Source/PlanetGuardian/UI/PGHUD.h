// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGHUD.generated.h"

class UPGGuardianHealthBarWidget;
class UPGJetpackPowerBarWidget;

UCLASS()
class PLANETGUARDIAN_API UPGHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPGGuardianHealthBarWidget> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPGJetpackPowerBarWidget> JetpackPowerBar;

public:
	UPGGuardianHealthBarWidget* GetHealthBar() const { return HealthBar; }

	UPGJetpackPowerBarWidget* GetJetpackPowerBar() const { return JetpackPowerBar; }
};
