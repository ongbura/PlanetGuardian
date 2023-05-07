// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "PGAbilitySystemGlobals.h"
#include "Input/PGAbilityInputData.h"
#include "InputAction.h"

UPGAbilitySystemComponent::UPGAbilitySystemComponent()
{
	AbilityInputHandles.Reserve(UPGAbilityInputData::MaxNumAbilityInputSet);
}

int32 UPGAbilitySystemComponent::GetSystemGlobalLevel()
{
	return UPGAbilitySystemGlobals::SystemGlobalLevel;
}

void UPGAbilitySystemComponent::BindAbilityToInput(const int32 InputID, const UInputAction* InputAction,
                                                   UEnhancedInputComponent* EIC)
{
	if (EIC == nullptr || InputAction == nullptr)
	{
		return;
	}
	
	auto& [OnPressedHandle, OnReleasedHandle] = AbilityInputHandles.Add(InputID);

	OnPressedHandle = &EIC->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnAbilityInputPressed, InputID);
	OnReleasedHandle = &EIC->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnAbilityInputReleased, InputID);
}

void UPGAbilitySystemComponent::UnbindAbilityFromInput(const int32 InputID, UEnhancedInputComponent* EIC)
{
	if (EIC == nullptr)
	{
		return;
	}

	FPGAbilityInputHandle HandleSet;
	
	AbilityInputHandles.RemoveAndCopyValue(InputID, HandleSet);

	EIC->RemoveBinding(*HandleSet.OnPressedHandle);
	EIC->RemoveBinding(*HandleSet.OnReleasedHandle);
}

void UPGAbilitySystemComponent::BindAbilitiesToInput(const UPGAbilityInputData* AbilityInputData,
                                                     UEnhancedInputComponent* EIC)
{
	if (EIC == nullptr || AbilityInputData == nullptr)
	{
		return;
	}
	
	for (const auto& InputSet : AbilityInputData->GetAbilityInputSets())
	{
		BindAbilityToInput(InputSet.InputID, InputSet.SoftInputAction.LoadSynchronous(), EIC);
	}
}

void UPGAbilitySystemComponent::ClearAbilitiesFromInput(UEnhancedInputComponent* EIC)
{
	if (EIC == nullptr)
	{
		return;
	}
	
	for (const auto& HandleSets : AbilityInputHandles)
	{
		EIC->RemoveBinding(*HandleSets.Value.OnPressedHandle);
		EIC->RemoveBinding(*HandleSets.Value.OnReleasedHandle);
	}

	AbilityInputHandles.Empty();
}

void UPGAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	// Called on server
	Super::OnGiveAbility(AbilitySpec);
}

void UPGAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	// Called on server	
	Super::OnRemoveAbility(AbilitySpec);
}

void UPGAbilitySystemComponent::OnAbilityInputPressed(const int32 InputID)
{
	AbilityLocalInputPressed(InputID);
}

void UPGAbilitySystemComponent::OnAbilityInputReleased(const int32 InputID)
{
	AbilityLocalInputReleased(InputID);
}
