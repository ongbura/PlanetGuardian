// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardian.h"

#include "EnhancedInputSubsystems.h"
#include "PGGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Input/PGInputComponent.h"

APGGuardian::APGGuardian()
	: CameraBoom(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")))
	  , FollowCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")))
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void APGGuardian::BeginPlay()
{
	Super::BeginPlay();
}

void APGGuardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APGGuardian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (InputConfig == nullptr)
	{
		return;
	}

	const auto* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (auto* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (auto* IC = CastChecked<UPGInputComponent>(PlayerInputComponent))
		{
			InputSubsystem->AddMappingContext(InputConfig->GetInputMappingContext(), InputConfig->GetInputMappingContextPriority());

			IC->BindActionByTag(InputConfig, FPGInputTags::Input_Action_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
			IC->BindActionByTag(InputConfig, FPGInputTags::Input_Action_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
			IC->BindActionByTag(InputConfig, FPGInputTags::Input_Action_Jump, ETriggerEvent::Triggered, this, &Super::Jump);
			IC->BindActionByTag(InputConfig, FPGInputTags::Input_Action_Jump, ETriggerEvent::Completed, this, &Super::StopJumping);
		}
	}
}

void APGGuardian::Move(const FInputActionValue& Value)
{
	const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
	const FRotator ControllerRotation{0.f, GetControlRotation().Yaw, 0.f};

	const auto ForwardDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
	AddMovementInput(ForwardDirection, AxisValue.Y);

	const auto RightDirection = ControllerRotation.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, AxisValue.X);
}

void APGGuardian::Look(const FInputActionValue& Value)
{
	const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
	AddControllerYawInput(AxisValue.X);
	AddControllerPitchInput(AxisValue.Y);
}
