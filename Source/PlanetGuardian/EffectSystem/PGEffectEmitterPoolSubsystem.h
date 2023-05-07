// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/PGWorldSubsystem.h"
#include "PGEffectEmitterPoolSubsystem.generated.h"


class APGEffectEmitterPool;

UCLASS()
class PLANETGUARDIAN_API UPGEffectEmitterPoolSubsystem : public UPGWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TWeakObjectPtr<APGEffectEmitterPool> EffectEmitterPool;

public:
	void SetEffectEmitterPool(APGEffectEmitterPool* InEffectEmitterPool);

	APGEffectEmitterPool* GetEffectEmitterPool() const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
