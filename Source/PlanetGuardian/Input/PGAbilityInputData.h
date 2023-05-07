// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PGGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "PGAbilityInputData.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FPGAbilityInputSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPGGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> SoftInputAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 InputID { 0 };

	friend uint32 GetTypeHash(const FPGAbilityInputSet& Info)
	{
		return GetTypeHash(Info.AbilityClass);
	}

	bool operator==(const FPGAbilityInputSet& Other) const
	{
		return AbilityClass == Other.AbilityClass && InputID == Other.InputID;
	}

	bool Equals(const FPGAbilityInputSet& Other) const
	{
		return AbilityClass == Other.AbilityClass && InputID == Other.InputID;
	}	
};

UCLASS()
class PLANETGUARDIAN_API UPGAbilityInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	static constexpr int32 MaxNumAbilityInputSet = 16;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSet<FPGAbilityInputSet> AbilityInputSets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TMap<TSoftObjectPtr<UInputMappingContext>, int32> AbilityInputMappingContexts;

public:
	UPGAbilityInputData();

	const auto& GetAbilityInputSets() const { return AbilityInputSets; }

	const auto& GetSoftAbilityInputMappingContexts() const { return AbilityInputMappingContexts; }

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
