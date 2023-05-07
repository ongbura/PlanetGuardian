// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EffectSystem/PGEffectSettings.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PGAnimNotify_EffectsEmitter.generated.h"

UCLASS(Abstract, HideCategories=Object, CollapseCategories)
class PLANETGUARDIAN_API UPGAnimNotify_EffectsEmitter : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FString NotifyDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPrimaryAssetId EffectBundle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_VFX VFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_SFX SFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bShouldAttached { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(AllowPrivateAccess="true", EditCondition="bShouldAttached", EditConditionHides))
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bPerformTrace { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_Trace TraceSettings;

protected:
	virtual FString GetNotifyName_Implementation() const override;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

private:
	void SpawnNiagaraEffect(UNiagaraSystem* NiagaraSystem, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	void SpawnSound(USoundBase* Sound, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
