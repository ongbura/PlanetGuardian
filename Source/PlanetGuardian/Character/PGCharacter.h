// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "PGCharacter.generated.h"

class UPGAbilitySystemComponent;
class UPGAvatarHelperComponent;

UCLASS()
class PLANETGUARDIAN_API APGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	DECLARE_EVENT(APGCharacter, FOnControllerChanged)
	DECLARE_EVENT(APGCharacter, FOnPlayerStateChanged)

public:
	FOnControllerChanged OnControllerChanged;
	FOnPlayerStateChanged OnPlayerStateChanged;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability")
	TWeakObjectPtr<UPGAbilitySystemComponent> AbilitySystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability")
	TObjectPtr<UPGAvatarHelperComponent> AvatarHelper;

public:
	APGCharacter();

	virtual bool IsAlive() const PURE_VIRTUAL(APGCharacter::IsAlive, return false;)

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPGAbilitySystemComponent* GetAbilitySystem() const { return AbilitySystem.Get(); }

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
};
