// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "PGEffector.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class PLANETGUARDIAN_API APGEffector : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraComponent> VisualFX;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> SoundFX;
	
public:
	APGEffector();	
};
