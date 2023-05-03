// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGHUD.generated.h"

class UPGStatusBarWidget;
/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPGStatusBarWidget> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPGStatusBarWidget> JetpackPowerBar;

public:
	UPGStatusBarWidget* GetHealthBar() const { return HealthBar; }

	UPGStatusBarWidget* GetJetpackPowerBar() const { return JetpackPowerBar; }
};
