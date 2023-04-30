// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "PGEffectorEmitter.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class USkeletalMeshComponent;

UCLASS()
class PLANETGUARDIAN_API APGEffectorEmitter : public AActor
{
	GENERATED_BODY()

	friend class UPGEffectSubsystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraComponent> VisualFX;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> SoundFX;

	bool bVisualFXEmittedComplete { true };

	bool bSoundFXEmittedComplete { true };

	bool bAttachedToMesh { false };

	bool bActivated { false };
	
public:
	APGEffectorEmitter();

	void Activate(const FTransform& Transform);

	void Activate(USkeletalMeshComponent* TargetMesh, const FName& SocketName);
	
protected:
	virtual void BeginPlay() override;
	
private:	
	void Activate();

	void Deactivate();
	
	void SetVisualEffect(UNiagaraSystem* VisualFXSystem) const;

	void SetSoundEffect(USoundBase* SoundFXSystem) const;

	UFUNCTION()
	void OnEndEmissionVisualFX(UNiagaraComponent* PSystem);

	UFUNCTION()
	void OnEndEmissionSoundFX();

	void TryReturnToEffectEmitterPool();
};
