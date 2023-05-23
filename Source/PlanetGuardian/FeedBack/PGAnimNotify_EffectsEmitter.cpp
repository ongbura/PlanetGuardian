// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAnimNotify_EffectsEmitter.h"
#include "NiagaraFunctionLibrary.h"
#include "EffectSystem/PGEffectBundle.h"
#include "EffectSystem/PGEffectEmitter.h"
#include "EffectSystem/PGEffectEmitterPool.h"
#include "Kismet/GameplayStatics.h"
#include "EffectSystem/PGEffectEmitterPoolSubsystem.h"
#include "EffectSystem/PGEffectSubsystem.h"
#include "System/PGAssetManager.h"

FString UPGAnimNotify_EffectsEmitter::GetNotifyName_Implementation() const
{
	if (NotifyDisplayName.IsEmpty())
	{
		return GetName();
	}

	return NotifyDisplayName;
}

void UPGAnimNotify_EffectsEmitter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp->GetOwnerRole() == ROLE_Authority)
	{
		return;
	}
	
	if (bSkipVisualFX && bSkipSoundFX)
	{
		Super::Notify(MeshComp, Animation, EventReference);
		return;
	}
	
	const auto* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const auto& AssetManager = UAssetManager::Get();

	if (World->WorldType == EWorldType::EditorPreview)
	{
		const auto* Bundle = Cast<UPGEffectBundle>(AssetManager.GetPrimaryAssetPath(EffectBundle).TryLoad());
		if (Bundle == nullptr)
		{
			return;
		}

		const auto VisualFXPath = Bundle->GetSoftVisualFXPath();
		if (VisualFXPath.IsValid())
		{
			if (auto* VisualFX = Cast<UNiagaraSystem>(Bundle->GetSoftVisualFXPath().TryLoad()))
			{
				SpawnNiagaraEffect(VisualFX, MeshComp, Animation);
			}
		}

		const auto SoundFXPath = Bundle->GetSoftSoundFXPath();
		if (SoundFXPath.IsValid())
		{
			if (auto* SoundFX = Cast<USoundBase>(Bundle->GetSoftSoundFXPath().TryLoad()))
			{
				SpawnSound(SoundFX, MeshComp, Animation);
			}
		}
		
		Super::Notify(MeshComp, Animation, EventReference);
		
		return;
	}

	const auto BundlePath = AssetManager.GetPrimaryAssetPath(EffectBundle);
	if (!BundlePath.IsValid())
	{
		return;
	}

	const auto* Bundle = AssetManager.GetPrimaryAssetObject<UPGEffectBundle>(EffectBundle);
	if (Bundle == nullptr)
	{
		Bundle = CastChecked<UPGEffectBundle>(BundlePath.TryLoad());
		check(Bundle);
	}

	if (!Bundle->GetSoftVisualFXPath().IsValid() && !Bundle->GetSoftSoundFXPath().IsValid())
	{
		return;
	}
	
	const auto* PoolSubsystem = World->GetSubsystem<UPGEffectEmitterPoolSubsystem>();
	if (PoolSubsystem == nullptr || PoolSubsystem->GetEffectEmitterPool() == nullptr)
	{		
		return;
	}

	auto* EffectSubsystem = World->GetSubsystem<UPGEffectSubsystem>();
	if (EffectSubsystem == nullptr)
	{
		return;
	}

	auto* VisualFX = bSkipVisualFX ? nullptr : EffectSubsystem->GetVisualFX(Bundle->GetSoftVisualFXPath());
	auto* SoundFX = bSkipSoundFX ? nullptr : EffectSubsystem->GetSoundFX(Bundle->GetSoftSoundFXPath());

	auto* Emitter = PoolSubsystem->GetEffectEmitterPool()->PopEffectEmitter();
	check(Emitter);
	
	Emitter->SetVisualEffectAndSettings(VisualFX, VFXSettings);
	Emitter->SetSoundEffectAndSettings(SoundFX, SFXSettings);

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
