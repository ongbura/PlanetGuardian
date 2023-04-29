// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputBindingComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PGAvatarComponent.h"
#include "Controller/PlayerController/PGPlayerController.h"
#include "InputMappingContext.h"

UPGInputBindingComponent::UPGInputBindingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	bWantsInitializeComponent = true;
}

void UPGInputBindingComponent::BindAbilityByTag(const FGameplayTag& AbilityTag)
{
	if (const auto* TaggedTypesManager = UPGAssistantSubsystem::Get())
	{
		
	}
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

UInputMappingContext* UPGInputBindingComponent::LoadInputMappingContext() const
{
	return InputMappingContext.LoadSynchronous();
}
