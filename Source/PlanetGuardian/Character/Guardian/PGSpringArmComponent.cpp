// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGSpringArmComponent.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"
#include "Components/CapsuleComponent.h"


UPGSpringArmComponent::UPGSpringArmComponent()
{
	bWantsInitializeComponent = true;
}

void UPGSpringArmComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CMC = Cast<UPGGuardianMovementComponent>(GetOwner()->GetComponentByClass(UPGGuardianMovementComponent::StaticClass()));
	check(CMC.IsValid());

	const auto* Capsule = Cast<UCapsuleComponent>(CMC->GetOwner()->FindComponentByClass(UCapsuleComponent::StaticClass()));
	check(Capsule);

	DefaultCapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	CrouchedCapsuleHalfHeight = CMC->GetCrouchedHalfHeight();

	InitialZLocation = GetRelativeLocation().Z;
}

void UPGSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CMC.IsValid())
	{
		return;
	}
	
	if (CMC->WasToggledCrouch())
	{
		CrouchBlendFinished = false;
	}
	
	if (CrouchBlendFinished)
	{
		return;
	}
	
	const float ZOffset = CMC->IsCrouching() ? CrouchedCapsuleHalfHeight - DefaultCapsuleHalfHeight : 0.f;
	const float TargetHeight = InitialZLocation + ZOffset;
	const float CurrentHeight = GetRelativeLocation().Z;
	
	if (FMath::IsNearlyEqual(CurrentHeight, TargetHeight))
	{
		CrouchBlendFinished = true;
		return;
	}

	float ResultHeight = FMath::FInterpTo(CurrentHeight, TargetHeight, DeltaTime, CrouchBlendSpeed);
	SetRelativeLocation({0.f, 0.f, ResultHeight}, false, nullptr, ETeleportType::None);
}

