// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PGPlayableCharacter.generated.h"

class UPGAbilitySystemComponent;
class UPGAvatarComponent;
class UPGInputBindingComponent;

UCLASS()
class PLANETGUARDIAN_API APGPlayableCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Avatar")
	TWeakObjectPtr<UPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Avatar")
	TObjectPtr<UPGAvatarComponent> AvatarComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Avatar")
	TObjectPtr<UPGInputBindingComponent> InputBindingComponent;

public:
	APGPlayableCharacter();

	virtual bool IsAlive() const PURE_VIRTUAL(APGCharacter::IsAlive, return false;)

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const { return AbilitySystemComponent.Get(); }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void OnRep_Controller() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
