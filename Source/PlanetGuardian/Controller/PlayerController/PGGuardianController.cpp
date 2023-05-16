// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianController.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "UI/PGHUD.h"
#include "UI/PGStatusBarWidget.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"
#include "System/PGCheatManager.h"

APGGuardianController::APGGuardianController()
{
	CheatClass = UPGCheatManager::StaticClass();
}

void APGGuardianController::MakeHUDVisible(UPGAbilitySystemComponent* ASC)
{
	if (HasAuthority() || ASC == nullptr)
	{
		return;
	}

	if (HUD == nullptr)
	{
		CreateHUD();
	}
	
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

void APGGuardianController::MakeHUDInvisible() const
{
	if (HUD)
	{
		HUD->RemoveFromParent();
	}
}

void APGGuardianController::CreateHUD()
{
	check(HUDClass);

	HUD = CreateWidget<UPGHUD>(this, HUDClass);
}
