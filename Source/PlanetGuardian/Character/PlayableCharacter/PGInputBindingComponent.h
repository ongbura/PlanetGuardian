// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGPawnAssistantComponent.h"
#include "EnhancedInputComponent.h"
#include "PGLogChannels.h"
#include "Subsystem/PGAssistantSubsystem.h"
#include "PGInputBindingComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class APGPlayerController;

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGInputBindingComponent : public UPGPawnAssistantComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 InputMappingContextPriority;

	TWeakObjectPtr<UEnhancedInputComponent> InputComponent;
	
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

public:
	UPGInputBindingComponent();
	
	template <typename UserClass, typename FuncType>
	void BindInputActionByTag(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	void BindAbilityByTag(const FGameplayTag& AbilityTag);

	void OnPlayerControllerAssigned(APGPlayerController* InController);

protected:
	virtual void InitializeComponent() override;

private:
	UInputMappingContext* LoadInputMappingContext() const;
};

template <typename UserClass, typename FuncType>
void UPGInputBindingComponent::BindInputActionByTag(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
                                               UserClass* Object, FuncType Func)
{
	if (!InputComponent.IsValid())
	{
		UE_LOG(PGInitialization, Error, TEXT("InputBindingComponent's Initialization has failed."));
		return;
	}

	if (const auto* TaggedTypesManager = UPGAssistantSubsystem::Get())
	{
		if (const auto* InputAction = TaggedTypesManager->FindInputAction(InputTag))
		{
			InputComponent->BindAction(InputAction, TriggerEvent, Object, Func);
		}
	}
	else
	{
		UE_LOG(PGInitialization, Error, TEXT("Can't find the ActionInput matching given tag."));
	}
}
