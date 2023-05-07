// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGEffectEmitter.generated.h"

struct FPGEffectSettings_SFX;
struct FPGEffectSettings_VFX;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class USkeletalMeshComponent;

UENUM()
enum class EPGEffectEmitterMode
{
	None		= 0,
	VisualOnly	= 1,
	SoundOnly	= 2,
	All			= 3
};

UCLASS()
class PLANETGUARDIAN_API APGEffectEmitter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraComponent> VisualFX;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> SoundFX;

	bool bVisualFXEmittedComplete { true };

	bool bSetVisualFX { false };

	bool bSoundFXEmittedComplete { true };

	bool bSetSoundFX { false };

	bool bAttachedToMesh { false };

	bool bActivated { false };

	EPGEffectEmitterMode EmitterMode { EPGEffectEmitterMode::None };
	
public:
	APGEffectEmitter();

	void Activate(const FTransform& Transform);

	void Activate(USkeletalMeshComponent* TargetMesh, const FName& SocketName);

	void SetVisualEffectSettings(UNiagaraSystem* Asset, const FPGEffectSettings_VFX& VisualFXSettings);

	void SetSoundEffectSettings(USoundBase* Asset, const FPGEffectSettings_SFX& SoundFXSettings);
	
protected:
	virtual void BeginPlay() override;
	
private:	
	void Activate();

	void ResetEffectEmitter();

	UFUNCTION()
	void OnEndEmissionVisualFX(UNiagaraComponent* PSystem);

	UFUNCTION()
	void OnEndEmissionSoundFX();

	void TryReturnToEffectEmitterPool();
};
