// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGActorPoolSubsystem.h"
#include "Cosmetics/PGEffectEmitterPool.h"

void UPGActorPoolSubsystem::SetEffectEmitterPool(APGEffectEmitterPool* InEffectEmitterPool)
{
	check(InEffectEmitterPool);
	EffectEmitterPool = InEffectEmitterPool;
}

APGEffectEmitterPool* UPGActorPoolSubsystem::GetEffectEmitterPool() const
{
	if (EffectEmitterPool.IsValid())
	{
		return EffectEmitterPool.Get();
	}

	return nullptr;
}

bool UPGActorPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}
