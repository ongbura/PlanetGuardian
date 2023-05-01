// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PGAttributeSet.h"
#include "PGAbilitySystemComponent.generated.h"

#define DEFAULT_ABILITY_LEVEL (1)

class UPGGameplayAbility;
class UGameplayEffect;
class UPGInputBindingComponent;

UCLASS()
class PLANETGUARDIAN_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UPGAbilitySystemComponent, FOnGiveAbility, FGameplayAbilitySpec&)
	DECLARE_EVENT_OneParam(UPGAbilitySystemComponent, FOnRemoveAbility, FGameplayAbilitySpec&)

public:
	FOnGiveAbility OnGiveAbilityEvent;
	FOnRemoveAbility OnRemoveAbilityEvent;

private:
	bool bStartupEffectsApplied{ false };

	TMap<FGameplayTag, TArray<FDelegateHandle>> AttributeChangedDelegateHandles;

	TWeakObjectPtr<UPGInputBindingComponent> InputBindingComponent;
	
public:
	UPGAbilitySystemComponent();

	template <typename UserClass, typename FuncType>
	void BindAttributeChangedDelegate(const UPGAttributeSet* AttributeSet, const FGameplayAttribute& Attribute, UserClass* User, FuncType Func);

	void RemoveBoundAttributeChangedDelegate(const UPGAttributeSet* AttributeSet);

	FORCEINLINE static int32 GetDefaultAbilityLevel() { return DEFAULT_ABILITY_LEVEL; }

	bool IsStartupEffectsApplied() const { return bStartupEffectsApplied; }

protected:
	virtual void BeginPlay() override;

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};

template <typename UserClass, typename FuncType>
void UPGAbilitySystemComponent::BindAttributeChangedDelegate(const UPGAttributeSet* AttributeSet, const FGameplayAttribute& Attribute, UserClass* User, FuncType Func)
{
	const auto& AttributeSetTag = AttributeSet->GetAttributeSetTag();	
	const auto Handle = GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(User, Forward(Func));

	if (AttributeChangedDelegateHandles.Contains(AttributeSetTag))
	{
		AttributeChangedDelegateHandles[AttributeSetTag].Add(MoveTemp(Handle));
	}
	else
	{
		AttributeChangedDelegateHandles.Add(AttributeSetTag, { MoveTemp(Handle) });
	}
}
