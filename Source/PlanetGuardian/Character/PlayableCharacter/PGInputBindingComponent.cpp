// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputBindingComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PGAvatarComponent.h"
#include "Controller/PlayerController/PGPlayerController.h"
#include "InputMappingContext.h"
#include "PGPlayableCharacter.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/PGGameplayAbility.h"
#include "DataAsset/PGInputActionAbilitySet.h"
#include "System/PGProjectSettings.h"

namespace InputBindingComponent_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

UPGInputBindingComponent::UPGInputBindingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UPGInputBindingComponent::BindAbilityInput(FGameplayAbilitySpec& Spec)
{
	using namespace InputBindingComponent_Impl;

	auto* Ability = Spec.Ability.Get();
	
	auto* AbilityBindingInfo = AbilityBindingInfos.Find(Ability);
	if (!AbilityBindingInfo)
	{
		AbilityBindingInfo = &AbilityBindingInfos.Add(Ability);
	}

	if (Spec.InputID == InvalidInputID)
	{
		Spec.InputID = GetNextInputID();
	}

	AbilityBindingInfo->AbilityHandle = Spec.Handle;
	const auto* InputAction = AbilityBindingInfo->InputAction.Get();

	check(InputComponent.IsValid());
	if (AbilityBindingInfo->OnPressedHandle == 0)
	{
		AbilityBindingInfo->OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this,
		                                                               &ThisClass::OnAbilityInputPressed,
		                                                               Spec.Handle).GetHandle();
	}

	if (AbilityBindingInfo->OnReleasedHandle == 0)
	{
		AbilityBindingInfo->OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this,
		                                                                &ThisClass::OnAbilityInputReleased,
		                                                                Spec.Handle).GetHandle();
	}
}

void UPGInputBindingComponent::UnbindAbilityInput(FGameplayAbilitySpec& Spec)
{
	using namespace InputBindingComponent_Impl;

	const auto* AbilityBindingInfo = AbilityBindingInfos.Find(Spec.Ability.Get());
	if (!AbilityBindingInfo)
	{
		return;
	}

	InputComponent->RemoveBindingByHandle(AbilityBindingInfo->OnPressedHandle);
	InputComponent->RemoveBindingByHandle(AbilityBindingInfo->OnReleasedHandle);

	AbilityBindingInfos.Remove(Spec.Ability.Get());
}

void UPGInputBindingComponent::OnPlayerControllerAssigned(APGPlayerController* InController)
{
	InputSubsystem = InController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(InputSubsystem.IsValid());

	InputComponent = Cast<UEnhancedInputComponent>(InController->InputComponent);
	check(InputComponent.IsValid());

	const auto* AxisInputMappingContext = AxisInputData.SoftInputMappingContext.LoadSynchronous();
	check(AxisInputMappingContext);

	InputSubsystem->AddMappingContext(AxisInputMappingContext, AxisInputData.InputMappingContextPriority);
}

void UPGInputBindingComponent::OnAbilitySystemInitialized(UPGAbilitySystemComponent* InASC)
{
	InASC->OnGiveAbilityEvent.AddUObject(this, &UPGInputBindingComponent::BindAbilityInput);
	InASC->OnRemoveAbilityEvent.AddUObject(this, &UPGInputBindingComponent::UnbindAbilityInput);
}

void UPGInputBindingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (const auto* Owner = GetPawnChecked<APawn>())
	{
		if (auto* AvatarComponent = Owner->FindComponentByClass<UPGAvatarComponent>())
		{
			AvatarComponent->OnPlayerControllerAssigned.AddUObject(this, &ThisClass::OnPlayerControllerAssigned);
			AvatarComponent->OnAbilitySystemInitialized.AddUObject(this, &ThisClass::OnAbilitySystemInitialized);
		}

		bIsOwnerLocallyControlled = Owner->IsLocallyControlled();
	}

	const auto* Settings = GetDefault<UPGProjectSettings>();
	if (Settings == nullptr)
	{
		return;
	}

	const auto* LoadedData = Settings->InputActionAbilityData.LoadSynchronous();
	if (LoadedData == nullptr)
	{
		return;
	}

	for (const auto& [InputAction, AbilityClass] : LoadedData->GetInputActionAbilityData())
	{
		auto* LoadedInputAction = InputAction.LoadSynchronous();
		check(LoadedInputAction);

		auto* AbilityCDO = Cast<UGameplayAbility>(AbilityClass.LoadSynchronous()->GetDefaultObject());
		check(AbilityCDO);
		
		FPGAbilityBindingInfo AbilityBindingInfo;
		AbilityBindingInfo.InputAction = LoadedInputAction;

		AbilityBindingInfos.Add(AbilityCDO, AbilityBindingInfo);
	}
}

void UPGInputBindingComponent::OnAbilityInputPressed(FGameplayAbilitySpecHandle SpecHandle)
{
	using namespace InputBindingComponent_Impl;

	auto* AbilitySystemComponent = GetAbilitySystemComponent();
	const auto* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);

	if (Spec != nullptr && Spec->InputID != InvalidInputID)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(Spec->InputID);
	}
}

void UPGInputBindingComponent::OnAbilityInputReleased(FGameplayAbilitySpecHandle SpecHandle)
{
	using namespace InputBindingComponent_Impl;

	auto* AbilitySystemComponent = GetAbilitySystemComponent();
	const auto* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);

	if (Spec != nullptr && Spec->InputID != InvalidInputID)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(Spec->InputID);
	}
}

UAbilitySystemComponent* UPGInputBindingComponent::GetAbilitySystemComponent() const
{
	const auto* Owner = Cast<IAbilitySystemInterface>(GetPawn<APGPlayableCharacter>());
	check(Owner);

	auto* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	return AbilitySystemComponent;
}
