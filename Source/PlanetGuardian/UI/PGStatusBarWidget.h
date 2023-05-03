// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "PGStatusBarWidget.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class PLANETGUARDIAN_API UPGStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusBar;

	float AttributeValue;
	FOnGameplayAttributeValueChange* AttributeChangedDelegate;
	FDelegateHandle AttributeChangedDelegateHandle;

	float MaxAttributeValue;
	FOnGameplayAttributeValueChange* MaxAttributeChangedDelegate;
	FDelegateHandle MaxAttributeChangedDelegateHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float StatusBarPercent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FLinearColor Color;

public:
	void InitializeStatusValue(float InAttributeValue, float InMaxAttributeValue);
	
	void BindAttributeChanged(FOnGameplayAttributeValueChange& Delegate);

	void BindMaxAttributeChanged(FOnGameplayAttributeValueChange& Delegate);

	void OnAttributeChanged(const FOnAttributeChangeData& ChangeData);

	void OnMaxAttributeChanged(const FOnAttributeChangeData& ChangeData);

protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
};
