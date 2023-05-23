// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianHealthBarWidget.h"

#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

void UPGGuardianHealthBarWidget::InitializeWithHealthSet(const UPGHealthSet* InHealthSet)
{
	check(InHealthSet);
	HealthSet = InHealthSet;

	HealthSet->BindAttributeChanged(UPGHealthSet::GetHealthAttribute(), this, &ThisClass::OnHealthChanged);
	HealthSet->BindAttributeChanged(UPGHealthSet::GetMaxHealthAttribute(), this, &ThisClass::OnMaxHealthChanged);

	SetPercent(HealthSet->GetHealth() / HealthSet->GetMaxHealth());
}

void UPGGuardianHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TopProgressBar->GetPercent() < BottomProgressBar->GetPercent())
	{
		const float NewPercent = UKismetMathLibrary::FInterpTo(BottomProgressBar->GetPercent(), TopProgressBar->GetPercent(), InDeltaTime, AnimationSpeed);
		BottomProgressBar->SetPercent(NewPercent);
	}
	else
	{
		BottomProgressBar->SetPercent(TopProgressBar->GetPercent());
	}
}

void UPGGuardianHealthBarWidget::NativeDestruct()
{
	if (HealthSet.IsValid())
	{
		HealthSet->UnbindAttributeChanged(UPGHealthSet::GetHealthAttribute(), this);
		HealthSet->UnbindAttributeChanged(UPGHealthSet::GetMaxHealthAttribute(), this);
	}
	
	Super::NativeDestruct();
}

void UPGGuardianHealthBarWidget::SetPercent(const float InPercent) const
{
	TopProgressBar->SetPercent(InPercent);
}

void UPGGuardianHealthBarWidget::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	const float NewPercent = Data.NewValue / HealthSet->GetMaxHealth();
	SetPercent(NewPercent);
}

void UPGGuardianHealthBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	const float NewPercent = HealthSet->GetHealth() / Data.NewValue;
	SetPercent(NewPercent);
}
