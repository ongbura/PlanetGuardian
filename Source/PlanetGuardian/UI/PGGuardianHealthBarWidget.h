// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "PGGuardianHealthBarWidget.generated.h"

class UPGHealthSet;
class UImage;
class UProgressBar;

UCLASS()
class PLANETGUARDIAN_API UPGGuardianHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> TopProgressBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> BottomProgressBar;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	float AnimationSpeed { 10.f };

	TWeakObjectPtr<const UPGHealthSet> HealthSet;

public:
	void InitializeWithHealthSet(const UPGHealthSet* InHealthSet);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

private:
	void SetPercent(float InPercent) const;
	
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
};
