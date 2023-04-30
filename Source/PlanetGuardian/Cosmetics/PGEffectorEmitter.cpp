// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectorEmitter.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Subsystem/PGEffectSubsystem.h"

// Sets default values
APGEffectorEmitter::APGEffectorEmitter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetHidden(true);
	SetActorTickEnabled(false);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	VisualFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VisualFX"));
	VisualFX->SetupAttachment(RootComponent);
	VisualFX->SetAutoActivate(false);

	SoundFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundFX"));
	SoundFX->SetupAttachment(RootComponent);
	SoundFX->SetAutoActivate(false);
}

void APGEffectorEmitter::Activate(const FTransform& Transform)
{
	SetActorTransform(Transform);

	Activate();
}

void APGEffectorEmitter::Activate(USkeletalMeshComponent* TargetMesh, const FName& SocketName)
{
	AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	bAttachedToMesh = true;

	Activate();
}

void APGEffectorEmitter::Activate()
{
	const bool bIsVisualFXValid = VisualFX->GetAsset() != nullptr;
	const bool bIsSoundFXValid = SoundFX->Sound != nullptr;
	
	if (bIsVisualFXValid || bIsSoundFXValid)
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);

		if (bIsVisualFXValid)
		{
			VisualFX->OnSystemFinished.AddDynamic(this, &APGEffectorEmitter::OnEndEmissionVisualFX);
			bVisualFXEmittedComplete = false;
			VisualFX->Activate();
		}

		if (bIsSoundFXValid)
		{
			SoundFX->OnAudioFinished.AddDynamic(this, &APGEffectorEmitter::OnEndEmissionSoundFX);
			bSoundFXEmittedComplete = false;
			SoundFX->Play();
		}

		bActivated = true;
	}
	else
	{
		TryReturnToEffectEmitterPool();
	}
}

void APGEffectorEmitter::Deactivate()
{
	if (!bActivated)
	{
		return;
	}

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
	const bool bIsVisualFXValid = VisualFX->GetAsset() != nullptr;
	const bool bIsSoundFXValid = SoundFX->Sound != nullptr;	

	if (bIsVisualFXValid)
	{
		VisualFX->ResetSystem();
		VisualFX->OnSystemFinished.Clear();
	}

	if (bIsSoundFXValid)
	{
		SoundFX->Sound = nullptr;
		SoundFX->ResetParameters();
		SoundFX->OnAudioFinished.Clear();
	}

	if (bAttachedToMesh)
	{
		DetachFromActor({ EDetachmentRule::KeepWorld, false });
		bAttachedToMesh = false;
	}

	bActivated = false;
}

void APGEffectorEmitter::SetVisualEffect(UNiagaraSystem* VisualFXSystem) const
{
	if (VisualFXSystem != nullptr)
	{
		VisualFX->SetAsset(VisualFXSystem);
	}
}

void APGEffectorEmitter::SetSoundEffect(USoundBase* SoundFXSystem) const
{
	if (SoundFXSystem != nullptr)
	{
		SoundFX->SetSound(SoundFXSystem);
	}
}

void APGEffectorEmitter::BeginPlay()
{
	Super::BeginPlay();
}

void APGEffectorEmitter::OnEndEmissionVisualFX(UNiagaraComponent* PSystem)
{
	VisualFX->ResetSystem();
	bVisualFXEmittedComplete = true;

	TryReturnToEffectEmitterPool();
}

void APGEffectorEmitter::OnEndEmissionSoundFX()
{
	SoundFX->SetSound(nullptr);
	SoundFX->ResetParameters();
	bSoundFXEmittedComplete = true;

	TryReturnToEffectEmitterPool();
}

void APGEffectorEmitter::TryReturnToEffectEmitterPool()
{
	if (!bVisualFXEmittedComplete || !bSoundFXEmittedComplete)
	{
		return;
	}

	Deactivate();

	if (auto* EffectSubsystem = UPGEffectSubsystem::Get())
	{
		EffectSubsystem->PushEffectEmitter(this);
	}
}
