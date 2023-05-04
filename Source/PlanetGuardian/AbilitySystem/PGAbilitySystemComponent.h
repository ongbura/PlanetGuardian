// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PGAttributeSet.h"
#include "PGAbilitySystemComponent.generated.h"

class UEnhancedInputComponent;
struct FEnhancedInputActionEventBinding;

USTRUCT()
struct FPGAbilityInputHandle
{
	GENERATED_BODY()

	FEnhancedInputActionEventBinding* OnPressedHandle {};

	FEnhancedInputActionEventBinding* OnReleasedHandle {};
};

UCLASS()
class PLANETGUARDIAN_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	inline static constexpr int32 SystemGlobalLevel { 1 };
	
private:
	TMap<FGameplayTag, TArray<FDelegateHandle>> AttributeChangedDelegateHandles;

	TMap<int32, FPGAbilityInputHandle> AbilityInputHandles;

	bool bAreAbilitiesBoundToInput;
	
public:
	UPGAbilitySystemComponent();

	static int32 GetSystemGlobalLevel() { return SystemGlobalLevel; } 

	template <typename UserClass, typename FuncType>
	void BindAttributeChangedDelegate(const UPGAttributeSet* AttributeSet, const FGameplayAttribute& Attribute, UserClass* User, FuncType Func);

	void RemoveBoundAttributeChangedDelegate(const UPGAttributeSet* AttributeSet);

	void BindDefaultAbilitiesToInputComponent(UEnhancedInputComponent* EIC);

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
	void OnAbilityInputPressed(int32 InputID);

	void OnAbilityInputReleased(int32 InputID);
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
