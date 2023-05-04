// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayableCharacter.h"
#include "PGAvatarComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Common/PGHealthSetComponent.h"
#include "Multiplayer/PGPlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APGPlayableCharacter::APGPlayableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	AvatarComponent = CreateDefaultSubobject<UPGAvatarComponent>(TEXT("AvaterComponent"));
	HealthComponent = CreateDefaultSubobject<UPGHealthSetComponent>(TEXT("HealthComponent"));
}

UAbilitySystemComponent* APGPlayableCharacter::GetAbilitySystemComponent() const
{
	return AvatarComponent->GetPGAbilitySystemComponent();
}

UPGAbilitySystemComponent* APGPlayableCharacter::GetPGAbilitySystemComponent() const
{
	return AvatarComponent->GetPGAbilitySystemComponent();
}

void APGPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APGPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init AbilitySystem on Server Side.

	if (auto* PS = GetPlayerStateChecked<APGPlayerState>())
	{
		AvatarComponent->InitializeAbilitySystem(PS->GetPGAbilitySystemComponent(), PS);

		if (GetNetMode() != NM_DedicatedServer && IsLocallyControlled())
		{
			AvatarComponent->HandlePlayerControllerAssigned();
		}
		
		// Some games grant attributes and abilities here

		// Some games server initialize another components of the character that use the ASC here
	}
}

void APGPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AvatarComponent->HandlePlayerStateAssigned();
}

void APGPlayableCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	AvatarComponent->HandlePlayerControllerAssigned();
}
