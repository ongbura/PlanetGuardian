// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PGAttributeSet.h"
#include "PGAbilitySystemComponent.generated.h"

class UEnhancedInputComponent;
struct FEnhancedInputActionEventBinding;
class UPGAbilityInputData;
class UInputAction;

USTRUCT()
struct FPGAbilityInputHandle
{
	GENERATED_BODY()

	FEnhancedInputActionEventBinding* OnPressedHandle;
	FEnhancedInputActionEventBinding* OnReleasedHandle;
};

UCLASS()
class PLANETGUARDIAN_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	TMap<int32, FPGAbilityInputHandle> AbilityInputHandles;
	
public:
	UPGAbilitySystemComponent();

	static int32 GetSystemGlobalLevel();

	void BindAbilityToInput(const int32 InputID, const UInputAction* InputAction, UEnhancedInputComponent* EIC);

	void UnbindAbilityFromInput(const int32 InputID, UEnhancedInputComponent* EIC);

	void BindAbilitiesToInput(const UPGAbilityInputData* AbilityInputData, UEnhancedInputComponent* EIC);

	void ClearAbilitiesFromInput(UEnhancedInputComponent* EIC);

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
	void OnAbilityInputPressed(int32 InputID);

	void OnAbilityInputReleased(int32 InputID);
};
