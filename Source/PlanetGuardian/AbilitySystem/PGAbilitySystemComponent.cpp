// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAbilitySystemComponent.h"

UPGAbilitySystemComponent::UPGAbilitySystemComponent()
{
}

void UPGAbilitySystemComponent::RemoveBoundAttributeChangedDelegate(const UPGAttributeSet* AttributeSet)
{
	const auto& AttributeSetTag = AttributeSet->GetAttributeSetTag();
	AttributeChangedDelegateHandles.Remove(AttributeSetTag);
}


// Called when the game starts
void UPGAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPGAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if (OnGiveAbilityEvent.IsBound())
	{
		OnGiveAbilityEvent.Broadcast(AbilitySpec);
	}
	
	Super::OnGiveAbility(AbilitySpec);
}

void UPGAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if (OnRemoveAbilityEvent.IsBound())
	{
		OnRemoveAbilityEvent.Broadcast(AbilitySpec);
	}
	
	Super::OnRemoveAbility(AbilitySpec);
}
