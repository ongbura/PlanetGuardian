// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardian.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "PGGuardianMovementComponent.h"
#include "PGJetpackPowerSetComponent.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Controller/PlayerController/PGGuardianController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/PGAbilityInputData.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputMappingContext.h"
#include "PGSpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystem/PGAvatarComponent.h"
#include "Character/Common/PGHealthSetComponent.h"
#include "Input/PGNativeInputData.h"
#include "Multiplayer/PGPlayerState.h"


APGGuardian::APGGuardian(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPGGuardianMovementComponent>(CharacterMovementComponentName))
	  , Jetpack(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack")))
	  , JetpackEffect(CreateDefaultSubobject<UNiagaraComponent>(TEXT("JetpackEffect")))
	  , JetpackSoundEffect(CreateDefaultSubobject<UAudioComponent>(TEXT("JetpackSound")))
{
	CameraBoom = CreateDefaultSubobject<UPGSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	GuardianMovementComponent = Cast<UPGGuardianMovementComponent>(GetCharacterMovement());
	GuardianMovementComponent->SetIsReplicated(true);

	JetpackPowerSetComponent = CreateDefaultSubobject<UPGJetpackPowerSetComponent>(TEXT("JetpackPowerSetComponent"));

	Jetpack->SetupAttachment(GetMesh(), TEXT("BackpackSocket"));

	JetpackEffect->SetupAttachment(Jetpack, TEXT("ThrusterSocketLeft"));
	JetpackEffect->SetAutoActivate(false);

	JetpackSoundEffect->SetupAttachment(RootComponent);
	JetpackSoundEffect->SetAutoActivate(false);

	AvatarComponent = CreateDefaultSubobject<UPGAvatarComponent>(TEXT("AvaterComponent"));
	HealthComponent = CreateDefaultSubobject<UPGHealthSetComponent>(TEXT("HealthComponent"));
}

UAbilitySystemComponent* APGGuardian::GetAbilitySystemComponent() const
{
	return AvatarComponent->GetPGAbilitySystemComponent();
}

UPGAbilitySystemComponent* APGGuardian::GetPGAbilitySystemComponent() const
{
	return AvatarComponent->GetPGAbilitySystemComponent();
}

void APGGuardian::ToggleJetpack(const bool bReset, const bool bActivate)
{
	// If reset, thruster time is set back to max thruster time
	if (bReset)
	{
		ResetThrusterTime();
	}

	bIsJetpackActivated = bActivate;

	// More air control while jetpack is active
	GetCharacterMovement()->AirControl = bIsJetpackActivated ? 5.f : 1.f;

	// The jetpack particle effect is (de)activated
	JetpackEffect->SetActive(bIsJetpackActivated);

	if (bIsJetpackActivated)
	{
		// The meta sound gets started. It has an ignite and a loop that is played until the JetpackOff event is triggered
		JetpackSoundEffect->Play();
	}
	else
	{
		// The parameter interface of the metasound only exists while playing.
		// So check before trying to pass data in.
		// When the jetpack goes off a trigger switches of the sound.
		// But instead just stop playing the looping part of the sound stops and a winding down sound is played.
		if (JetpackSoundEffect->IsPlaying())
		{
			JetpackSoundEffect->SetTriggerParameter(TEXT("JetpackOff"));
		}
	}
}

void APGGuardian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto* PC = GetController<APlayerController>();
	check(PC);

	auto* EIS = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EIS);

	if (const auto* NativeInputMappingContext = NativeInputData->GetSoftNativeInputMappingContext().LoadSynchronous())
	{
		EIS->AddMappingContext(NativeInputMappingContext, NativeInputData->GetNativeInputMappingContextPriority());
	}

	for (const auto& SoftMappingContext : AbilityInputData->GetSoftAbilityInputMappingContexts())
	{
		if (const auto* MappingContext = SoftMappingContext.Key.LoadSynchronous())
		{
			const auto Priority = SoftMappingContext.Value;
			EIS->AddMappingContext(MappingContext, Priority);
		}
	}

	auto* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	if (const auto* MoveAction = NativeInputData->GetSoftMoveInputAction().LoadSynchronous())
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}

	if (const auto* LookAction = NativeInputData->GetSoftLookInputAction().LoadSynchronous())
	{
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}

	auto* AbilitySystem = GetPGAbilitySystemComponent();

	if (AbilityInputData != nullptr && AbilitySystem != nullptr)
	{
		AbilitySystem->BindAbilitiesToInput(AbilityInputData, Cast<UEnhancedInputComponent>(InputComponent));
	}
}

void APGGuardian::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (auto* PS = GetPlayerStateChecked<APGPlayerState>())
	{
		// Initialize the ability system component
		AvatarComponent->InitializeAbilitySystem(PS->GetPGAbilitySystemComponent(), PS);

		if (GetNetMode() != NM_DedicatedServer && IsLocallyControlled())
		{
			AvatarComponent->HandlePlayerControllerAssigned();
		}
		
		auto* AbilitySystem = GetPGAbilitySystemComponent();

		// Give the default abilities
		// If the ability input data is valid, the abilities are bound to the input
		if (AbilityInputData)
		{
			auto CopiedDefaultAbilities = DefaultAbilities;
			const auto& AbilityInputSets = AbilityInputData->GetAbilityInputSets();

			for (const auto& [AbilityClass, SoftInputAction, InputID] : AbilityInputSets)
			{
				if (DefaultAbilities.Contains(AbilityClass))
				{
					FGameplayAbilitySpec Spec
					{
						AbilityClass,
						AbilitySystem->GetSystemGlobalLevel(),
						InputID,
						this
					};

					AbilitySystem->GiveAbility(Spec);
					CopiedDefaultAbilities.Remove(AbilityClass);
				}
			}

			for (const auto& AbilityClass : CopiedDefaultAbilities)
			{
				AbilitySystem->GiveAbility({ AbilityClass });
			}
		}
		else
		{
			for (const auto& AbilityClass : DefaultAbilities)
			{
				AbilitySystem->GiveAbility({ AbilityClass });
			}
		}
	}
}

void APGGuardian::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AvatarComponent->HandlePlayerStateAssigned();

	if (AbilityInputData == nullptr || InputComponent != nullptr)
	{
		return;
	}

	auto* AbilitySystem = GetPGAbilitySystemComponent();
	AbilitySystem->BindAbilitiesToInput(AbilityInputData, Cast<UEnhancedInputComponent>(InputComponent));
}

void APGGuardian::OnRep_Controller()
{
	Super::OnRep_Controller();

	AvatarComponent->HandlePlayerControllerAssigned();
	
	if (auto* PC = GetController<APGGuardianController>())
	{
		PC->MakeHUDVisible(GetPGAbilitySystemComponent());
	}
}

void APGGuardian::BeginPlay()
{
	Super::BeginPlay();

	ResetThrusterTime();

	LandedDelegate.AddDynamic(this, &APGGuardian::OnLandedToggleJetpack);
}

void APGGuardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UpdateCamera(DeltaTime);
	UpdateJetpack(DeltaTime);
}

void APGGuardian::UpdateCamera(const float DeltaSeconds)
{
	if (const auto* World = GetWorld())
	{
		FHitResult HitResult;

		const FVector Start = GetActorLocation();
		const FVector End = Start + FVector { 0.f, 0.f, 250.f };

		World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera,
		                                FCollisionQueryParams { NAME_None, false, this });

		float LerpTarget = GetCharacterMovement()->IsFalling() ? 600.f : 500.f;

		if (HitResult.bBlockingHit)
		{
			LerpTarget = 300.f;
		}

		const float TargetArmLengthThisFrame = UKismetMathLibrary::FInterpTo(
			CameraBoom->TargetArmLength, LerpTarget, DeltaSeconds, 2.f);

		CameraBoom->TargetArmLength = TargetArmLengthThisFrame;
	}
}

void APGGuardian::UpdateJetpack(const float DeltaSeconds)
{
	if (JetpackBoostCurve == nullptr || !bIsJetpackActivated)
	{
		return;
	}

	// When Jetpack is active decrease the thruster time
	ThrusterTime -= DeltaSeconds;

	// Using ThrusterMaxTime to get a value between 0 and 1 and use that in a curve to get the launch velocity on the character.
	// Now you can modify the curve to get the wanted behaviour.
	// The bot is only launched a bit at start but then the jetpack depletes.
	const float NormalizedTime = UKismetMathLibrary::NormalizeToRange(ThrusterMaxTime - ThrusterTime, 0.f,
	                                                                  ThrusterMaxTime);
	const float JetpackBoost = JetpackBoostCurve->GetFloatValue(NormalizedTime);
	LaunchCharacter({ 0.f, 0.f, JetpackBoost * 120.f }, false, true);

	// The particle effect samples the sockets and has a variable that can be updated.
	// So the thrust amount is reflected in the visible effect.
	// You could also feed this data into the sound if you want.
	JetpackEffect->SetNiagaraVariableFloat(TEXT("ThrusterStrength"), JetpackBoost);

	// When thruster time is 0 the jetpack is toggled off
	if (ThrusterTime <= 0.f)
	{
		ToggleJetpack(false, false);
	}
}

void APGGuardian::OnLandedToggleJetpack(const FHitResult& Hit)
{
	ToggleJetpack(true, false);
}

void APGGuardian::Move(const FInputActionValue& Value)
{
	const auto MoveInput = Value.Get<FInputActionValue::Axis2D>();

	const FRotator CameraWorldRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardBasedXInput(FRotationMatrix(CameraWorldRotation).GetScaledAxis(EAxis::X) * MoveInput.X);
	const FVector RightBasedYInput(FRotationMatrix(CameraWorldRotation).GetScaledAxis(EAxis::Y) * MoveInput.Y);

	FVector DirectionToMove(ForwardBasedXInput + RightBasedYInput);
	DirectionToMove.Normalize();

	const float MoveAmount = FMath::Abs(MoveInput.X) > FMath::Abs(MoveInput.Y)
								 ? FMath::Abs(MoveInput.X)
								 : FMath::Abs(MoveInput.Y);

	AddMovementInput(DirectionToMove, MoveAmount);
}

void APGGuardian::Look(const FInputActionValue& Value)
{
	if (auto* PC = Cast<APlayerController>(GetController()))
	{
		const auto AxisValue = Value.Get<FInputActionValue::Axis2D>();
		PC->AddYawInput(AxisValue.X);
		PC->AddPitchInput(AxisValue.Y);
	}
}
