// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputBindingComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PGAvatarComponent.h"
#include "PGNativeGameplayTags.h"
#include "Controller/PlayerController/PGPlayerController.h"
#include "Input/PGInputConfig.h"
#include "InputAction.h"
#include "InputMappingContext.h"

UPGInputBindingComponent::UPGInputBindingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	bWantsInitializeComponent = true;
}

void UPGInputBindingComponent::OnPlayerControllerAssigned(APGPlayerController* InController)
{
	InputSubsystem = InController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem.IsValid());

	InputComponent = Cast<UEnhancedInputComponent>(InController->InputComponent);
	check(InputComponent.IsValid());

	InputSubsystem->AddMappingContext(LoadInputMappingContext(), InputMappingContextPriority);
}

void UPGInputBindingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (const auto* Owner = GetPawnChecked<APawn>())
	{
		if (auto* AvatarComponent = Owner->FindComponentByClass<UPGAvatarComponent>())
		{
			AvatarComponent->OnPlayerControllerAssigned.AddUObject(this, &ThisClass::OnPlayerControllerAssigned);
		}
	}
}

UInputAction* UPGInputBindingComponent::LoadInputActionForTag(const FGameplayTag& Tag) const
{
	if (const auto* Found = InputActions.Find({ nullptr, Tag }))
	{
		return Found->InputAction.LoadSynchronous();
	}

	return nullptr;
}

UInputMappingContext* UPGInputBindingComponent::LoadInputMappingContext() const
{
	return InputMappingContext.LoadSynchronous();
}
