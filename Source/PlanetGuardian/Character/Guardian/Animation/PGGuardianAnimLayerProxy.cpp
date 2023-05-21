// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianAnimLayerProxy.h"
#include "PGGuardianAnimInstance.h"
#include "PGGuardianAnimLayer.h"
#include "Character/Guardian/PGGuardian.h"
#include "GameFramework/CharacterMovementComponent.h"

FPGGuardianAnimLayerProxy::FPGGuardianAnimLayerProxy(UAnimInstance* InAnimInstance)
	: Super(InAnimInstance)
{
}

void FPGGuardianAnimLayerProxy::Initialize(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::Initialize(InAnimInstance);

	AnimLayer = Cast<UPGGuardianAnimLayer>(InAnimInstance);
	check(AnimLayer.IsValid());

	const auto* OwningComponent = InAnimInstance->GetOwningComponent();
	if (OwningComponent == nullptr)
	{
		return;
	}

	if (const auto* AnimInstance = Cast<UPGGuardianAnimInstance>(OwningComponent->GetAnimInstance()))
	{
		AnimLayer->LinkedInstance = AnimInstance;

		const auto* Guardian = OwningComponent->GetOwner<APGGuardian>();
		if (Guardian == nullptr)
		{
			return;
		}

		CMC = Guardian->GetCharacterMovement();
	}
	else
	{
		AnimLayer->LinkedInstance.Reset();
		AnimLayer.Reset();
		CMC.Reset();
	}
}

void FPGGuardianAnimLayerProxy::Uninitialize(UAnimInstance* InAnimInstance)
{
	if (AnimLayer.IsValid())
	{
		AnimLayer->LinkedInstance.Reset();
	}

	AnimLayer.Reset();
	CMC.Reset();
	
	FAnimInstanceProxy::Uninitialize(InAnimInstance);
}

void FPGGuardianAnimLayerProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);

	if (!CMC.IsValid())
	{
		return;
	}

	LastUpdateVelocity = CMC->GetLastUpdateVelocity();
	CurrentAcceleration = CMC->GetCurrentAcceleration();
	BrakingFriction = CMC->BrakingFriction;
	GroundFriction = CMC->GroundFriction;
	BrakingFrictionFactor = CMC->BrakingFrictionFactor;
	BrakingDecelerationWalking = CMC->BrakingDecelerationWalking;
	bUseSeparateBrakingFriction = CMC->bUseSeparateBrakingFriction;	
}
