// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectEmitterPoolSubsystem.h"
#include "EffectSystem/PGEffectEmitterPool.h"

void UPGEffectEmitterPoolSubsystem::SetEffectEmitterPool(APGEffectEmitterPool* InEffectEmitterPool)
{
	check(InEffectEmitterPool);
	EffectEmitterPool = InEffectEmitterPool;
}

APGEffectEmitterPool* UPGEffectEmitterPoolSubsystem::GetEffectEmitterPool() const
{
	if (EffectEmitterPool.IsValid())
	{
		return EffectEmitterPool.Get();
	}

	return nullptr;
}

void UPGEffectEmitterPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UPGEffectEmitterPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return IsRunningDedicatedServer() ? false : true;
}
