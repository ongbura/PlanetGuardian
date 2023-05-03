// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGStatusBarWidget.h"

#include "Components/Image.h"

void UPGStatusBarWidget::InitializeStatusValue(float InAttributeValue, float InMaxAttributeValue)
{
	AttributeValue = InAttributeValue;
	MaxAttributeValue = InMaxAttributeValue;
	StatusBarPercent = AttributeValue / MaxAttributeValue;
}

void UPGStatusBarWidget::BindAttributeChanged(FOnGameplayAttributeValueChange& Delegate)
{
	AttributeChangedDelegateHandle = Delegate.AddUObject(this, &UPGStatusBarWidget::OnAttributeChanged);
	AttributeChangedDelegate = &Delegate;
}

void UPGStatusBarWidget::BindMaxAttributeChanged(FOnGameplayAttributeValueChange& Delegate)
{
	MaxAttributeChangedDelegateHandle = Delegate.AddUObject(this, &UPGStatusBarWidget::OnMaxAttributeChanged);
	MaxAttributeChangedDelegate = &Delegate;
}

void UPGStatusBarWidget::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	AttributeValue = ChangeData.NewValue;
	StatusBarPercent = AttributeValue / MaxAttributeValue;
}

void UPGStatusBarWidget::OnMaxAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxAttributeValue = ChangeData.NewValue;
	StatusBarPercent = AttributeValue / MaxAttributeValue;
}

void UPGStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPGStatusBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	AttributeChangedDelegate->Remove(AttributeChangedDelegateHandle);
	MaxAttributeChangedDelegate->Remove(MaxAttributeChangedDelegateHandle);
}
