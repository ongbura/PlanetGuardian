// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGPlayableCharacter.h"
#include "PGAvatarComponent.h"
#include "PGInputBindingComponent.h"
#include "PGNativeGameplayTags.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Multiplayer/PGPlayerState.h"

APGPlayableCharacter::APGPlayableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AvatarComponent = CreateDefaultSubobject<UPGAvatarComponent>(TEXT("AvaterComponent"));
	InputBindingComponent = CreateDefaultSubobject<UPGInputBindingComponent>(TEXT("InputBindingComponent"));
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

	InputBindingComponent->BindMoveAction(ETriggerEvent::Triggered, this, &ThisClass::Move);
	InputBindingComponent->BindLookAction(ETriggerEvent::Triggered, this, &ThisClass::Look);
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
			AvatarComponent->GrantDefaultAbilitiesAndApplyStartupEffects();
		}

		
		// Some games grant attributes and abilities here

		// Some games server initialize another components of the character that use the ASC here
	}
}

void APGPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AvatarComponent->HandlePlayerStateAssigned();
	AvatarComponent->GrantDefaultAbilitiesAndApplyStartupEffects();
}

void APGPlayableCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	AvatarComponent->HandlePlayerControllerAssigned();
}

void APGPlayableCharacter::Move(const FInputActionValue& Value)
{
	const auto MoveInput = Value.Get<FInputActionValue::Axis2D>();

	const FRotator CameraWorldRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardBasedXInput(FRotationMatrix(CameraWorldRotation).GetScaledAxis(EAxis::X) * MoveInput.X);
	const FVector RightBasedYInput(FRotationMatrix(CameraWorldRotation).GetScaledAxis(EAxis::Y) * MoveInput.Y);

	FVector DirectionToMove(ForwardBasedXInput + RightBasedYInput);
	DirectionToMove.Normalize();

	const float MoveAmount= FMath::Abs(MoveInput.X) > FMath::Abs(MoveInput.Y) ? FMath::Abs(MoveInput.X) : FMath::Abs(MoveInput.Y);
	
	AddMovementInput(DirectionToMove, MoveAmount);
}

void APGPlayableCharacter::Look(const FInputActionValue& Value)
{
	if (auto* PC = Cast<APlayerController>(GetController()))
	{
		const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
		PC->AddYawInput(AxisValue.X);
		PC->AddPitchInput(AxisValue.Y);
	}
}
