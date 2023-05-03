// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardian.h"
#include "NiagaraComponent.h"
#include "PGGuardianMovementComponent.h"
#include "PGJetpackPowerSetComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

APGGuardian::APGGuardian(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPGGuardianMovementComponent>(CharacterMovementComponentName))
	, Jetpack(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack")))
	, JetpackEffect(CreateDefaultSubobject<UNiagaraComponent>(TEXT("JetpackEffect")))
	, JetpackSoundEffect(CreateDefaultSubobject<UAudioComponent>(TEXT("JetpackSound")))
{
	GuardianMovementComponent = Cast<UPGGuardianMovementComponent>(GetCharacterMovement());
	GuardianMovementComponent->SetIsReplicated(true);

	JetpackPowerSetComponent = CreateDefaultSubobject<UPGJetpackPowerSetComponent>(TEXT("JetpackPowerSetComponent"));

	Jetpack->SetupAttachment(GetMesh(), TEXT("BackpackSocket"));

	JetpackEffect->SetupAttachment(Jetpack, TEXT("ThrusterSocketLeft"));
	JetpackEffect->SetAutoActivate(false);

	JetpackSoundEffect->SetupAttachment(RootComponent);
	JetpackSoundEffect->SetAutoActivate(false);
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
		const FVector End = Start + FVector{ 0.f, 0.f, 250.f};
		
		World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera, FCollisionQueryParams{ NAME_None, false, this });

		float LerpTarget = GetCharacterMovement()->IsFalling() ? 600.f : 500.f;
		
		if (HitResult.bBlockingHit)
		{
			LerpTarget = 300.f;
		}

		const float TargetArmLengthThisFrame = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, LerpTarget, DeltaSeconds, 2.f);

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
	const float NormalizedTime = UKismetMathLibrary::NormalizeToRange(ThrusterMaxTime - ThrusterTime, 0.f, ThrusterMaxTime);
	const float JetpackBoost = JetpackBoostCurve->GetFloatValue(NormalizedTime);
	LaunchCharacter({0.f, 0.f, JetpackBoost * 120.f}, false, true);

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
