// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianController.h"
#include "Input/PGInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Guardian/PGGuardian.h"
#include "PGNativeGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

APGGuardianController::APGGuardianController()
{
}

void APGGuardianController::BeginPlay()
{
	Super::BeginPlay();
}

void APGGuardianController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!ensureMsgf(InputConfig != nullptr, TEXT("InputConfig is nullptr!")))
	{
		return;
	}

	if (auto* InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (auto* IC = CastChecked<UPGInputComponent>(InputComponent))
		{
			InputSubsystem->AddMappingContext(InputConfig->GetInputMappingContext(),
			                                  InputConfig->GetInputMappingContextPriority());

			IC->BindActionByTag(InputConfig, FPGNativeGameplayTags::Input_Action_Move, ETriggerEvent::Triggered, this,
			                    &ThisClass::Move);
			IC->BindActionByTag(InputConfig, FPGNativeGameplayTags::Input_Action_Look, ETriggerEvent::Triggered, this,
			                    &ThisClass::Look);
			IC->BindActionByTag(InputConfig, FPGNativeGameplayTags::Input_Action_Jump, ETriggerEvent::Started, this,
			                    &ThisClass::Jump);
			IC->BindActionByTag(InputConfig, FPGNativeGameplayTags::Input_Action_Jump, ETriggerEvent::Completed, this,
			                    &ThisClass::StopJumping);
		}
	}
}

void APGGuardianController::Move(const FInputActionValue& Value)
{
	RETURN_PAWN_IF_NULL();

	const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
	const FRotator ControllerRotation{ 0.f, GetControlRotation().Yaw, 0.f };

	const auto ForwardDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
	GetPawn()->AddMovementInput(ForwardDirection, AxisValue.Y);

	const auto RightDirection = ControllerRotation.RotateVector(FVector::RightVector);
	GetPawn()->AddMovementInput(RightDirection, AxisValue.X);
}

void APGGuardianController::Look(const FInputActionValue& Value)
{
	const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
	AddYawInput(AxisValue.X);
	AddPitchInput(AxisValue.Y);
}

void APGGuardianController::Jump(const FInputActionValue& Value)
{
	RETURN_PAWN_IF_NULL();

	if (auto* Guardian = Cast<APGGuardian>(GetPawn()))
	{
		switch (Guardian->GetCharacterMovement()->MovementMode)
		{
		case MOVE_Walking:
			[[fallthrough]];
		case MOVE_NavWalking:
			Guardian->Jump();
			break;
		case MOVE_Falling:
			[[fallthrough]];
		case MOVE_Flying:
			Guardian->ToggleJetpack(false, true);
			break;
		default:
			checkNoEntry();
		}
		Guardian->Jump();
	}
}

void APGGuardianController::StopJumping(const FInputActionValue& Value)
{
	RETURN_PAWN_IF_NULL();

	if (auto* Guardian = Cast<APGGuardian>(GetPawn()))
	{
		Guardian->StopJumping();
		Guardian->ToggleJetpack(false, false);
	}
}
