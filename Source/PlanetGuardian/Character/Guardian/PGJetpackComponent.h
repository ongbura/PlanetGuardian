// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "PGJetpackComponent.generated.h"

class UNiagaraSystem;


UCLASS()
class PLANETGUARDIAN_API UPGJetpackComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraSystem> JetpackEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USoundBase> JetpackSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCurveFloat> JetpackBoostCurve;

public:
	
};
