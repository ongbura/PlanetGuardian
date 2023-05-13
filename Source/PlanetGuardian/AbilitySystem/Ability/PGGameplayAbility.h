// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PGGameplayAbility.generated.h"



/**
 * @brief Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EPGAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS(Abstract)
class PLANETGUARDIAN_API UPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPGAbilityActivationPolicy ActivationPolicy;

public:
	UPGGameplayAbility();

	template <typename T>
	T* GetAbilitySystemComponentFromActorInfo() const;

	template <typename T>
	T* GetPlayerControllerFromActorInfo() const;

	template <typename T>
	T* GetCharacterFromActorInfo() const;

	EPGAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }	

	static int32 GetSystemGlobalLevel();

protected:
	// If an ability is marked as 'bShouldActivateOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};

template <typename T>
T* UPGGameplayAbility::GetAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo ? Cast<T>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

template <typename T>
T* UPGGameplayAbility::GetPlayerControllerFromActorInfo() const
{
	return CurrentActorInfo ? Cast<T>(CurrentActorInfo->PlayerController.Get()) : nullptr;
}

template <typename T>
T* UPGGameplayAbility::GetCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<T>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}
