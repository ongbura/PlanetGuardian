// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PGPlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPGAbilitySystemComponent;
class UPGAvatarComponent;
class UPGInputBindingComponent;
class UPGHealthSetComponent;

UCLASS()
class PLANETGUARDIAN_API APGPlayableCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UPGAvatarComponent> AvatarComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UPGInputBindingComponent> InputBindingComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UPGHealthSetComponent> HealthComponent;

public:
	explicit APGPlayableCharacter(const FObjectInitializer& ObjectInitializer);

	virtual bool IsAlive() const PURE_VIRTUAL(APGCharacter::IsAlive, return false;)

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void OnRep_Controller() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
