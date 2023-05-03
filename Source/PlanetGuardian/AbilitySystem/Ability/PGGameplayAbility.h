// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bShouldActivateOnGranted { false };

public:
	UPGGameplayAbility();

protected:
	// If an ability is marked as 'bShouldActivateOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
