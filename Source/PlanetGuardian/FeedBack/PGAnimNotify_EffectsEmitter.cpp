// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAnimNotify_EffectsEmitter.h"

#include "Cosmetics/PGEffectorEmitter.h"
#include "Subsystem/PGEffectSubsystem.h"

FString UPGAnimNotify_EffectsEmitter::GetNotifyName_Implementation() const
{
	return EffectName;
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
	Super::Notify(MeshComp, Animation, EventReference);
	
	auto* EffectSubsystem = UPGEffectSubsystem::Get();
	if (EffectSubsystem == nullptr)
	{
		return;
	}
	
	auto* Emitter = EffectSubsystem->PopEffectEmitter(EffectSet);
	check(Emitter);

	if (bShouldAttached)
	{
		Emitter->Activate(MeshComp, SocketName);
	}
	else
	{
		const auto EffectTransform = MeshComp->GetSocketTransform(SocketName);
		Emitter->Activate(EffectTransform);
	}
}
