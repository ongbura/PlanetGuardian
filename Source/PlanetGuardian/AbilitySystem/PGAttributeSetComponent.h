// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGAttributeSetComponent.generated.h"

struct FOnAttributeChangeData;
class UPGAttributeSet;
class UPGAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPGOnAttributeChanged, UPGAbilitySystemComponent*, Owner, AActor*, Instigator, float, NewValue, float, OldValue);

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGAttributeSetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UPGAbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<const UPGAttributeSet> AttributeSet;

public:
	UPGAttributeSetComponent();

	virtual void InitializeWithAbilitySystem(UPGAbilitySystemComponent* ASC);

	virtual void UninitializeFromAbilitySystem();

protected:
	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;
	
	static AActor* GetInstigatorFromAttrChangeDate(const FOnAttributeChangeData& ChangeData);

	template <typename T>
	T* GetAttributeSet() const;	
};

template <typename T>
T* UPGAttributeSetComponent::GetAttributeSet() const
{
	if (AttributeSet.IsValid())
	{
		return Cast<T>(AttributeSet.Get());
	}

	return nullptr;
}
