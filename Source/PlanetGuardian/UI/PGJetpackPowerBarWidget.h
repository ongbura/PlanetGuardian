// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "PGJetpackPowerBarWidget.generated.h"

class UPGJetpackPowerSet;
class UImage;
class UProgressBar;

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGJetpackPowerBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	TWeakObjectPtr<const UPGJetpackPowerSet> JetpackPowerSet;

public:
	void InitializeWithJetpackPowerSet(const UPGJetpackPowerSet* InJetpackPowerSet);

protected:
	virtual void NativeDestruct() override;
	
private:
	void SetPercent(float InPercent) const;
	
	void OnJetpackPowerChanged(const FOnAttributeChangeData& Data) const;
	
	void OnMaxJetpackPowerChanged(const FOnAttributeChangeData& Data) const;
};
