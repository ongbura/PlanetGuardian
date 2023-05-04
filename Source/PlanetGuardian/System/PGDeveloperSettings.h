// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "PGDeveloperSettings.generated.h"

class UGameplayEffect;
class UPGEffectSetData;
class UPGGameplayAbility;

USTRUCT(BlueprintType)
struct FPGAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPGGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> InputAction;

	int32 InputID;

	friend uint32 GetTypeHash(const FPGAbilitySet& Info)
	{
		return GetTypeHash(Info.AbilityClass);
	}

	bool operator==(const FPGAbilitySet& Other) const
	{
		return AbilityClass == Other.AbilityClass && InputID == Other.InputID;
	}

	bool Equals(const FPGAbilitySet& Other) const
	{
		return AbilityClass == Other.AbilityClass && InputID == Other.InputID;
	}
};

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Planet Guardian Settings"))
class PLANETGUARDIAN_API UPGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Guardian | Input Configuration")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Guardian | Input Configuration")
	TSoftObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Guardian | Input Configuration")
	TSoftObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Guardian | Ability System")
	TSet<FPGAbilitySet> DefaultAbilities;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Guardian | Ability System")
	TArray<TSoftClassPtr<UGameplayEffect>> InitialEffects;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 EffectEmitterPoolCapacity { 256 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 NumInitialEffectEmitters { 64 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect | Emitter Pool")
	int32 NumNewEffectEmittersPerFrame { 8 };

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Cosmetic Configuration | Effect", meta=(RelativeToGameContentDir, LongPackageName))
	FDirectoryPath PlayEffectPath;

	UPGDeveloperSettings() = default;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
