// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EffectSystem/PGEffectSettings.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PGAnimNotify_EffectsEmitter.generated.h"

UCLASS(Abstract)
class PLANETGUARDIAN_API UPGAnimNotify_EffectsEmitter : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString NotifyDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId EffectBundle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPGEffectSettings_VFX VFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPGEffectSettings_SFX SFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldAttached { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bShouldAttached", EditConditionHides))
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPerformTrace { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPGEffectSettings_Trace TraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSkipVisualFX { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSkipSoundFX { false };

protected:
	virtual FString GetNotifyName_Implementation() const override;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

private:
	void SpawnNiagaraEffect(UNiagaraSystem* NiagaraSystem, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	void SpawnSound(USoundBase* Sound, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
