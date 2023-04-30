// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGEffectSettings.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PGAnimNotify_EffectsEmitter.generated.h"

class UPGEffectSetData;

UCLASS(Abstract, HideCategories=Object, CollapseCategories)
class PLANETGUARDIAN_API UPGAnimNotify_EffectsEmitter : public UAnimNotify
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeleton> Skeleton;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FString EffectName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector LocationOffset { FVector::ZeroVector };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FRotator RotationOffset { FRotator::ZeroRotator };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UPGEffectSetData> EffectSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_VFX VFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_SFX SFXSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bShouldAttached { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bLoadEffectsAsynchronous { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(AllowPrivateAccess="true", EditCondition="bShouldAttached", EditConditionHides))
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bPerformTrace { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGEffectSettings_Trace TraceSettings;

protected:
	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
