// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAnimNotify_EffectsEmitter.h"
#include "NiagaraFunctionLibrary.h"
#include "Cosmetics/PGEffectEmitter.h"
#include "Cosmetics/PGEffectEmitterPool.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/PGActorPoolSubsystem.h"
#include "Subsystem/PGEffectSubsystem.h"

FString UPGAnimNotify_EffectsEmitter::GetNotifyName_Implementation() const
{
	if (NotifyDisplayName.IsEmpty())
	{
		return GetName();
	}

	return NotifyDisplayName;
}

#if WITH_EDITOR
bool UPGAnimNotify_EffectsEmitter::CanBePlaced(UAnimSequenceBase* Animation) const
{
	if (Animation == nullptr)
	{
		return false;
	}

	if (Skeleton == nullptr)
	{
		return false;
	}

	if (Skeleton != Animation->GetSkeleton())
	{
		return false;
	}

	return true;
}
#endif

void UPGAnimNotify_EffectsEmitter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	const auto* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const auto* PoolSubsystem = World->GetSubsystem<UPGActorPoolSubsystem>();
	if (PoolSubsystem == nullptr || PoolSubsystem->GetEffectEmitterPool() == nullptr)
	{
		auto* VisualFX = VFXSettings.NiagaraFX.LoadSynchronous();
		auto* SoundFX = SFXSettings.SoundFX.LoadSynchronous();
		
		SpawnSound(SoundFX, MeshComp, Animation);
		SpawnNiagaraEffect(VisualFX, MeshComp, Animation);
		Super::Notify(MeshComp, Animation, EventReference);

		return;
	}

	auto* EffectSubsystem = World->GetGameInstance()->GetSubsystem<UPGEffectSubsystem>();
	if (EffectSubsystem == nullptr)
	{
		return;
	}

	auto* Emitter = PoolSubsystem->GetEffectEmitterPool()->PopEffectEmitter();
	check(Emitter);

	Emitter->SetVisualEffect(EffectSubsystem->FindOrLoadNiagaraSystem(VFXSettings.NiagaraFX));
	Emitter->SetSoundEffect(EffectSubsystem->FindOrLoadSoundBase(SFXSettings.SoundFX));

	if (bShouldAttached)
	{
		Emitter->Activate(MeshComp, SocketName);
	}
	else
	{
		const auto EffectTransform = MeshComp->GetSocketTransform(SocketName);
		Emitter->Activate(EffectTransform);
	}

	Super::Notify(MeshComp, Animation, EventReference);
}

void UPGAnimNotify_EffectsEmitter::SpawnNiagaraEffect(UNiagaraSystem* NiagaraSystem, USkeletalMeshComponent* MeshComp,
                                                      UAnimSequenceBase* Animation)
{
	if (bShouldAttached)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, MeshComp, SocketName, VFXSettings.LocationOffset,
		                                             VFXSettings.RotationOffset, EAttachLocation::KeepRelativeOffset,
		                                             true);
	}
	else
	{
		const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), NiagaraSystem,
		                                               MeshTransform.TransformPosition(VFXSettings.LocationOffset),
		                                               (MeshTransform.GetRotation() * VFXSettings.RotationOffset.
			                                               Quaternion()).Rotator(), VFXSettings.Scale, true);
	}
}

void UPGAnimNotify_EffectsEmitter::SpawnSound(USoundBase* Sound, USkeletalMeshComponent* MeshComp,
                                              UAnimSequenceBase* Animation)
{
	if (bShouldAttached)
	{
		UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, SocketName, FVector(ForceInit),
		                                     EAttachLocation::SnapToTarget, false, SFXSettings.VolumeMultiplier,
		                                     SFXSettings.PitchMultiplier);
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation(),
		                                      SFXSettings.VolumeMultiplier, SFXSettings.PitchMultiplier);
	}
}
