// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGJetpackPowerBarWidget.h"

#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"
#include "Components/ProgressBar.h"

void UPGJetpackPowerBarWidget::InitializeWithJetpackPowerSet(const UPGJetpackPowerSet* InJetpackPowerSet)
{
	check(InJetpackPowerSet);
	JetpackPowerSet = InJetpackPowerSet;

	JetpackPowerSet->BindAttributeChanged(UPGJetpackPowerSet::GetJetpackPowerAttribute(), this, &ThisClass::OnJetpackPowerChanged);
	JetpackPowerSet->BindAttributeChanged(UPGJetpackPowerSet::GetMaxJetpackPowerAttribute(), this, &ThisClass::OnMaxJetpackPowerChanged);

	SetPercent(JetpackPowerSet->GetJetpackPower() / JetpackPowerSet->GetMaxJetpackPower());
}

void UPGJetpackPowerBarWidget::NativeDestruct()
{
	if (JetpackPowerSet.IsValid())
	{
		JetpackPowerSet->UnbindAttributeChanged(UPGJetpackPowerSet::GetJetpackPowerAttribute(), this);
		JetpackPowerSet->UnbindAttributeChanged(UPGJetpackPowerSet::GetMaxJetpackPowerAttribute(), this);
	}
	
	Super::NativeDestruct();
}

void UPGJetpackPowerBarWidget::SetPercent(float InPercent) const
{
	ProgressBar->SetPercent(InPercent);
}

void UPGJetpackPowerBarWidget::OnJetpackPowerChanged(const FOnAttributeChangeData& Data) const
{
	const float NewPercent = Data.NewValue / JetpackPowerSet->GetMaxJetpackPower();
	SetPercent(NewPercent);
}

void UPGJetpackPowerBarWidget::OnMaxJetpackPowerChanged(const FOnAttributeChangeData& Data) const
{
	const float NewPercent = JetpackPowerSet->GetJetpackPower() / Data.NewValue;
	SetPercent(NewPercent);
}
