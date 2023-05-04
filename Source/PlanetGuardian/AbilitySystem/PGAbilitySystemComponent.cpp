// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "System/PGDeveloperSettings.h"

UPGAbilitySystemComponent::UPGAbilitySystemComponent()
{
}

void UPGAbilitySystemComponent::RemoveBoundAttributeChangedDelegate(const UPGAttributeSet* AttributeSet)
{
	const auto& AttributeSetTag = AttributeSet->GetAttributeSetTag();
	AttributeChangedDelegateHandles.Remove(AttributeSetTag);
}

void UPGAbilitySystemComponent::BindDefaultAbilitiesToInputComponent(UEnhancedInputComponent* EIC)
{
	if (bAreAbilitiesBoundToInput)
	{
		return;
	}
	
	auto* Settings = GetDefault<UPGDeveloperSettings>();

	for (const auto& [SoftAbility, SoftInputAction, InputID] : Settings->DefaultAbilities)
	{
		const auto* InputAction = SoftInputAction.LoadSynchronous();

		auto& [OnPressedHandle, OnReleasedHandle] = AbilityInputHandles.Add(InputID);
		OnPressedHandle = &EIC->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnAbilityInputPressed, InputID);
		OnReleasedHandle = &EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnAbilityInputReleased, InputID);
	}

	bAreAbilitiesBoundToInput = true;
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