// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectEmitter.h"
#include "NiagaraComponent.h"
#include "PGEffectEmitterPool.h"
#include "Components/AudioComponent.h"
#include "PGEffectSettings.h"
#include "PGEffectEmitterPoolSubsystem.h"
#include "ThirdParty/MagicEnum/magic_enum.hpp"

// Sets default values
APGEffectEmitter::APGEffectEmitter()
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

void APGEffectEmitter::Activate(const FTransform& Transform)
{
	SetActorTransform(Transform);

	Activate();
}

void APGEffectEmitter::Activate(USkeletalMeshComponent* TargetMesh, const FName& SocketName)
{
	AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	bAttachedToMesh = true;

	Activate();
}

void APGEffectEmitter::SetVisualEffectAndSettings(UNiagaraSystem* Asset, const FPGEffectSettings_VFX& VisualFXSettings)
{
	if (Asset == nullptr)
	{
		return;
	}

	AddActorLocalOffset(VisualFXSettings.LocationOffset);
	AddActorLocalRotation(VisualFXSettings.RotationOffset);
	SetActorScale3D(VisualFXSettings.Scale);
		
	VisualFX->SetAsset(Asset);
	bSetVisualFX = true;
}

void APGEffectEmitter::SetSoundEffectAndSettings(USoundBase* Asset, const FPGEffectSettings_SFX& SoundFXSettings)
{
	if (Asset == nullptr)
	{
		return;
	}

	SoundFX->VolumeMultiplier = SoundFXSettings.VolumeMultiplier;
	SoundFX->PitchMultiplier = SoundFXSettings.PitchMultiplier;
		
	SoundFX->SetSound(Asset);
	bSetSoundFX = true;
}

void APGEffectEmitter::BeginPlay()
{
	Super::BeginPlay();
}

void APGEffectEmitter::Activate()
{
	const int32 ModeFlag = static_cast<int32>(bSetVisualFX) | static_cast<int32>(bSetSoundFX << 1);
	EmitterMode = magic_enum::enum_cast<EPGEffectEmitterMode>(ModeFlag).value();

	if (bSetVisualFX || bSetSoundFX)
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);

		if (bSetVisualFX)
		{
			VisualFX->OnSystemFinished.AddDynamic(this, &APGEffectEmitter::OnEndEmissionVisualFX);
			bVisualFXEmittedComplete = false;
			VisualFX->Activate(true);
		}

		if (bSetSoundFX)
		{
			SoundFX->OnAudioFinished.AddDynamic(this, &APGEffectEmitter::OnEndEmissionSoundFX);
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

void APGEffectEmitter::ResetEffectEmitter()
{
	if (!bActivated)
	{
		return;
	}

	if (bSetVisualFX)
	{
		VisualFX->Deactivate();
		VisualFX->OnSystemFinished.RemoveAll(this);
		bSetVisualFX = false;
	}

	if (bSetSoundFX)
	{
		SoundFX->ResetParameters();
		SoundFX->Sound = nullptr;
		SoundFX->OnAudioFinished.RemoveAll(this);
		bSetSoundFX = false;
	}

	if (bAttachedToMesh)
	{
		DetachFromActor({ EDetachmentRule::KeepWorld, false });
		bAttachedToMesh = false;
	}

	bActivated = false;
	EmitterMode = EPGEffectEmitterMode::None;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void APGEffectEmitter::OnEndEmissionVisualFX(UNiagaraComponent* PSystem)
{
	VisualFX->DeactivateImmediate();
	bVisualFXEmittedComplete = true;

	TryReturnToEffectEmitterPool();
}

void APGEffectEmitter::OnEndEmissionSoundFX()
{
	SoundFX->Stop();
	bSoundFXEmittedComplete = true;

	TryReturnToEffectEmitterPool();
}

void APGEffectEmitter::TryReturnToEffectEmitterPool()
{
	ResetEffectEmitter();

	auto* EmitterPool = GetWorld()->GetSubsystem<UPGEffectEmitterPoolSubsystem>()->GetEffectEmitterPool();
	check(EmitterPool);

	switch (EmitterMode)
	{
	case EPGEffectEmitterMode::None:
		EmitterPool->PushEffectEmitter(this);
		return;
	case EPGEffectEmitterMode::VisualOnly:
		verify(bVisualFXEmittedComplete && !bSoundFXEmittedComplete);
	case EPGEffectEmitterMode::SoundOnly:
		verify(!bVisualFXEmittedComplete && bSoundFXEmittedComplete);
	case EPGEffectEmitterMode::All:
		if (!bVisualFXEmittedComplete || !bSoundFXEmittedComplete)
		{
			return;
		}
		break;
	default:
		checkNoEntry();
	}

	EmitterPool->PushEffectEmitter(this);
}
