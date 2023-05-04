// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectEmitterPool.h"
#include "PGEffectEmitter.h"
#include "Subsystem/PGActorPoolSubsystem.h"
#include "System/PGDeveloperSettings.h"

APGEffectEmitterPool::APGEffectEmitterPool()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	if (auto* Settings = GetDefault<UPGDeveloperSettings>())
	{
		EmitterPool = MakeUnique<TAtomicQueue<APGEffectEmitter*>>(Settings->EffectEmitterPoolCapacity);
	}
}

APGEffectEmitter* APGEffectEmitterPool::PopEffectEmitter()
{
	if (auto* Emitter = EmitterPool->pop())
	{
		return Emitter;
	}

	auto* Settings = GetDefault<UPGDeveloperSettings>();
	check(Settings);

	static int32 NumNewEmitters = Settings->NumNewEffectEmittersPerFrame;

	if (static_cast<uint32>(AllEffectEmitters.Num() + NumNewEmitters) > EmitterPool->capacity())
	{
		ensureMsgf(false, TEXT("Pool Size is not enough. Increase pool size or decrease number of effect emitters."));
		return nullptr;
	}

	GenerateEffectEmitters(NumNewEmitters);
	auto* Emitter = EmitterPool->pop();
	check(Emitter);

	return Emitter;
}

void APGEffectEmitterPool::PushEffectEmitter(APGEffectEmitter* Emitter)
{
	EmitterPool->push(Emitter);
}

void APGEffectEmitterPool::BeginPlay()
{
	Super::BeginPlay();
	
	if (const auto* Settings = GetDefault<UPGDeveloperSettings>())
	{
		GenerateEffectEmitters(Settings->NumInitialEffectEmitters);
	}

	if (auto* PoolSubsystem = GetWorld()->GetSubsystem<UPGActorPoolSubsystem>())
	{
		PoolSubsystem->SetEffectEmitterPool(this);
	}
}

void APGEffectEmitterPool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (auto* Elem : AllEffectEmitters)
	{
		Elem->ConditionalBeginDestroy();
	}
}

void APGEffectEmitterPool::GenerateEffectEmitters(const int32 NumEmitters)
{
	for (int32 i = 0; i < NumEmitters; ++i)
	{
		auto* Emitter = Cast<APGEffectEmitter>(GetWorld()->SpawnActor(APGEffectEmitter::StaticClass()));
		check(Emitter);

		{
			FScopeLock Lock(&EmitterPoolLockGuard);
			AllEffectEmitters.Add(Emitter);
		}
		
		EmitterPool->push(Emitter);
	}
}


