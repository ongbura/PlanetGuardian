// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGWorldSubsystem.h"
#include "PGActorPoolSubsystem.generated.h"


class APGEffectEmitterPool;

UCLASS()
class PLANETGUARDIAN_API UPGActorPoolSubsystem : public UPGWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TWeakObjectPtr<APGEffectEmitterPool> EffectEmitterPool;

public:
	void SetEffectEmitterPool(APGEffectEmitterPool* InEffectEmitterPool);

	APGEffectEmitterPool* GetEffectEmitterPool() const;

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
