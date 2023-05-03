// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayerController.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "UI/PGHUD.h"
#include "UI/PGStatusBarWidget.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"

void APGPlayerController::CreateHUD(UPGAbilitySystemComponent* ASC)
{
	if (HUD != nullptr || !HUDClass || !IsLocalPlayerController())
	{
		return;
	}

	HUD = CreateWidget<UPGHUD>(this, HUDClass);
	HUD->AddToViewport();

	auto* HealthBar = HUD->GetHealthBar();
	check(HealthBar);

	auto&& HealthAttribute = UPGHealthSet::GetHealthAttribute();
	auto&& MaxHealthAttribute = UPGHealthSet::GetMaxHealthAttribute();

	HealthBar->InitializeStatusValue(ASC->GetNumericAttributeBase(HealthAttribute), ASC->GetNumericAttributeBase(MaxHealthAttribute));
	HealthBar->BindAttributeChanged(ASC->GetGameplayAttributeValueChangeDelegate(MoveTemp(HealthAttribute)));
	HealthBar->BindAttributeChanged(ASC->GetGameplayAttributeValueChangeDelegate(MoveTemp(MaxHealthAttribute)));

	auto* JetpackPowerBar = HUD->GetJetpackPowerBar();
	check(JetpackPowerBar);

	auto&& JetpackPowerAttribute = UPGJetpackPowerSet::GetJetpackPowerAttribute();
	auto&& MaxJetpackPowerAttribute = UPGJetpackPowerSet::GetMaxJetpackPowerAttribute();

	JetpackPowerBar->InitializeStatusValue(ASC->GetNumericAttributeBase(JetpackPowerAttribute), ASC->GetNumericAttributeBase(MaxJetpackPowerAttribute));
	JetpackPowerBar->BindAttributeChanged(ASC->GetGameplayAttributeValueChangeDelegate(MoveTemp(JetpackPowerAttribute)));
	JetpackPowerBar->BindAttributeChanged(ASC->GetGameplayAttributeValueChangeDelegate(MoveTemp(MaxJetpackPowerAttribute)));
}

void APGPlayerController::DestroyHUD()
{
	if (HUD)
	{
		HUD->RemoveFromParent();
		HUD->ConditionalBeginDestroy();
	}
}

void APGPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
}
