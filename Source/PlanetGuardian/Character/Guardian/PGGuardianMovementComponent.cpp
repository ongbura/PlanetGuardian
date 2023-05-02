﻿// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianMovementComponent.h"

#include "GameFramework/Character.h"


UPGGuardianMovementComponent::UPGGuardianMovementComponent()
{
}

void UPGGuardianMovementComponent::Sprint()
{
	bWantToSprint = true;
	bToggledSprint = true;
}

void UPGGuardianMovementComponent::StopSprinting()
{
	bWantToSprint = false;
	bToggledSprint = true;
}

FNetworkPredictionData_Client* UPGGuardianMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		auto* MutableThis = const_cast<UPGGuardianMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Guardian(*this);
	}

	return ClientPredictionData;
}

void UPGGuardianMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UPGGuardianMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (bToggledSprint && MovementMode == MOVE_Walking)
	{
		const float Temp = MaxWalkSpeed;
		MaxWalkSpeed = MaxSprintSpeed;
		MaxSprintSpeed = Temp;

		bToggledSprint = false;
	}
}

FSavedMove_Guardian::FSavedMove_Guardian()
	: bWantsToSprint(false)
{
}

bool FSavedMove_Guardian::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const auto* NewGuardianMove = static_cast<FSavedMove_Guardian*>(NewMove.Get());

	if (bWantsToSprint != NewGuardianMove->bWantsToSprint)
	{
		return false;
	}

	return true;
}

void FSavedMove_Guardian::Clear()
{
	FSavedMove_Character::Clear();

	bWantsToSprint = false;
}

uint8 FSavedMove_Guardian::GetCompressedFlags() const
{
	auto Result = FSavedMove_Character::GetCompressedFlags();

	if (bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void FSavedMove_Guardian::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (const auto* CMC = Cast<UPGGuardianMovementComponent>(C->GetCharacterMovement()))
	{
		bWantsToSprint = CMC->WantsToSprint();
	}
}

void FSavedMove_Guardian::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	if (auto* CMC = Cast<UPGGuardianMovementComponent>(C->GetCharacterMovement()))
	{
		CMC->SetWantToSprint(bWantsToSprint);
	}
}

FNetworkPredictionData_Client_Guardian::FNetworkPredictionData_Client_Guardian(const UCharacterMovementComponent& ClientMovement)
	: FNetworkPredictionData_Client_Character(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_Guardian::AllocateNewMove()
{
	return MakeShared<FSavedMove_Guardian>();
}