// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/Common/PGCharacter.h"
#include "PGPlayableCharacter.generated.h"

struct FInputActionValue;
class UPGAbilitySystemComponent;
class UPGAvatarComponent;
class UPGHealthSetComponent;
class UPGNativeInputData;

UCLASS()
class PLANETGUARDIAN_API APGPlayableCharacter : public APGCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability System")
	TObjectPtr<UPGAvatarComponent> AvatarComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability System")
	TObjectPtr<UPGHealthSetComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Input")
	TObjectPtr<UPGNativeInputData> NativeInputData;

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
	/**
	* @brief Keyboard wasd Movement
	*/
	void Move(const FInputActionValue& Value);

	/**
	 * @brief Mouse look
	 */
	void Look(const FInputActionValue& Value);
};
