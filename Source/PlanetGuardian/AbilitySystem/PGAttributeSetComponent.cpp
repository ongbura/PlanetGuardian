// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAttributeSetComponent.h"
#include "PGAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Character/PlayableCharacter/PGAvatarComponent.h"


UPGAttributeSetComponent::UPGAttributeSetComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UPGAttributeSetComponent::InitializeWithAbilitySystem(UPGAbilitySystemComponent* ASC)
{
	check(ASC);
	AbilitySystemComponent = ASC;
}

void UPGAttributeSetComponent::UninitializeFromAbilitySystem()
{
}

void UPGAttributeSetComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const auto* Owner = GetOwner();

	auto* AvatarComponent = Owner->FindComponentByClass<UPGAvatarComponent>();
	check(AvatarComponent);

	AvatarComponent->OnAbilitySystemInitialized.AddUObject(this, &UPGAttributeSetComponent::InitializeWithAbilitySystem);
}

void UPGAttributeSetComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

AActor* UPGAttributeSetComponent::GetInstigatorFromAttrChangeDate(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}
