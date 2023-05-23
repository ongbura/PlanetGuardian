// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianController.h"
#include "AbilitySystem/AttributeSet/PGHealthSet.h"
#include "UI/PGHUD.h"
#include "UI/PGStatusBarWidget.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PGJetpackPowerSet.h"
#include "Camera/PGGuardianCameraManager.h"
#include "Character/Guardian/PGGuardian.h"
#include "System/PGCheatManager.h"
#include "UI/PGGuardianHealthBarWidget.h"
#include "UI/PGJetpackPowerBarWidget.h"

APGGuardianController::APGGuardianController()
{
	CheatClass = UPGCheatManager::StaticClass();
}

void APGGuardianController::MakeHUDVisible(const UPGAbilitySystemComponent* ASC)
{
	if (ASC == nullptr || HUDClass == nullptr)
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

	HealthBar->InitializeWithHealthSet(ASC->GetSet<UPGHealthSet>());

	auto* JetpackPowerBar = HUD->GetJetpackPowerBar();
	check(JetpackPowerBar);

	JetpackPowerBar->InitializeWithJetpackPowerSet(ASC->GetSet<UPGJetpackPowerSet>());
}

void APGGuardianController::MakeHUDInvisible() const
{
	if (HUD)
	{
		HUD->RemoveFromParent();
	}
}

void APGGuardianController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (IsLocalController())
	{
		const auto* Guardian = Cast<APGGuardian>(P);
		auto* PCM = Cast<APGGuardianCameraManager>(PlayerCameraManager);
		check(PCM);

		PCM->InitializeObjects(Guardian);
	}
}

void APGGuardianController::CreateHUD()
{
	check(HUDClass);

	HUD = CreateWidget<UPGHUD>(this, HUDClass);
}
