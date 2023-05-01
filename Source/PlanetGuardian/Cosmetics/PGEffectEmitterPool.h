// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdParty/AtomicQueue/atomic_queue.h"
#include "PGEffectEmitterPool.generated.h"

class APGEffectEmitter;

UCLASS(DisplayName="EffectEmitterPool")
class PLANETGUARDIAN_API APGEffectEmitterPool : public AActor
{
	GENERATED_BODY()

	template <typename T>
	using TAtomicQueue = atomic_queue::AtomicQueueB<T>;

	UPROPERTY(Transient)
	TArray<APGEffectEmitter*> AllEffectEmitters;

	TUniquePtr<TAtomicQueue<APGEffectEmitter*>> EmitterPool;

	FCriticalSection EmitterPoolLockGuard;
	
public:
	APGEffectEmitterPool();

	APGEffectEmitter* PopEffectEmitter();

	void PushEffectEmitter(APGEffectEmitter* Emitter);	

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	void GenerateEffectEmitters(int32 NumEmitters);
};
