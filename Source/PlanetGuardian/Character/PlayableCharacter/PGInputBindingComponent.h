// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGPawnAssistantComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "PGLogChannels.h"
#include "PGInputBindingComponent.generated.h"

class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class APGPlayerController;
class UPGAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayAbility;

struct FGameplayAbilitySpec;

USTRUCT()
struct FPGAbilityBindingInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UInputAction> InputAction;

	int32 InputID { 0 };

	uint32 OnPressedHandle { 0 };

	uint32 OnReleasedHandle { 0 };

	FGameplayAbilitySpecHandle AbilityHandle {};
};

USTRUCT(BlueprintType)
struct FPGAxisInputData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> SoftMoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> SoftLookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputMappingContext> SoftInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InputMappingContextPriority { 0 };
};

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGInputBindingComponent : public UPGPawnAssistantComponent
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TMap<UGameplayAbility*, FPGAbilityBindingInfo> AbilityBindingInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FPGAxisInputData AxisInputData;

	TWeakObjectPtr<UEnhancedInputComponent> InputComponent;

	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

	bool bIsOwnerLocallyControlled;

public:
	UPGInputBindingComponent();

	template <typename UserClass, typename FuncType>
	void BindMoveAction(ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template <typename UserClass, typename FuncType>
	void BindLookAction(ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	void BindAbilityInput(FGameplayAbilitySpec& Spec);

	void UnbindAbilityInput(FGameplayAbilitySpec& Spec);

	void OnPlayerControllerAssigned(APGPlayerController* InController);

	void OnAbilitySystemInitialized(UPGAbilitySystemComponent* InASC);

protected:
	virtual void InitializeComponent() override;

private:
	void OnAbilityInputPressed(FGameplayAbilitySpecHandle SpecHandle);

	void OnAbilityInputReleased(FGameplayAbilitySpecHandle SpecHandle);

	UAbilitySystemComponent* GetAbilitySystemComponent() const;
};

template <typename UserClass, typename FuncType>
void UPGInputBindingComponent::BindMoveAction(ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	if (!InputComponent.IsValid() || !AxisInputData.SoftMoveInputAction.IsValid())
	{
		return;
	}

	auto* MoveInputAction = AxisInputData.SoftMoveInputAction.LoadSynchronous();
	check(MoveInputAction);

	InputComponent->BindAction(MoveInputAction, TriggerEvent, Object, Func);
}

template <typename UserClass, typename FuncType>
void UPGInputBindingComponent::BindLookAction(ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	if (!InputComponent.IsValid())
	{
		UE_LOG(PGInitialization, Error, TEXT("InputBindingComponent's Initialization has failed."));
		return;
	}

	auto* LookInputAction = AxisInputData.SoftLookInputAction.LoadSynchronous();
	check(LookInputAction);

	InputComponent->BindAction(LookInputAction, TriggerEvent, Object, Func);
}
