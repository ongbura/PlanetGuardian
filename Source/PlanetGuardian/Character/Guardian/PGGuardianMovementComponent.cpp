// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


UPGGuardianMovementComponent::UPGGuardianMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

void UPGGuardianMovementComponent::SprintPressed()
{
	bWantsToSprint = true;
}

bool UPGGuardianMovementComponent::CanSprint() const
{
	return (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking) && !IsCrouching() && !IsFalling();
}

void UPGGuardianMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
}

void UPGGuardianMovementComponent::ToggleCrouch()
{
	bWantsToCrouchOld = bWantsToCrouch;
	bWantsToCrouch = !bWantsToCrouch;
}

const FPGGuardianJumpFallData& UPGGuardianMovementComponent::GetJumpFallData()
{
	if (!CharacterOwner || (GFrameCounter == JumpFallData.LastUpdateFrame))
	{
		return JumpFallData;
	}

	if (MovementMode == MOVE_Walking)
	{
		JumpFallData.GroundHitResult = CurrentFloor.HitResult;
		JumpFallData.DistanceFromGround = 0.0f;
	}
	else
	{
		static constexpr float GroundTraceDistance = 100000.f;
		
		const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LyraCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		JumpFallData.GroundHitResult = HitResult;
		JumpFallData.DistanceFromGround = GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			JumpFallData.DistanceFromGround = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			JumpFallData.DistanceFromGround = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	JumpFallData.LastUpdateFrame = GFrameCounter;

	return JumpFallData;
}

FNetworkPredictionData_Client* UPGGuardianMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		auto* MutableThis = const_cast<UPGGuardianMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Guardian(*this);
		check(ClientPredictionData);
	}

	return ClientPredictionData;
}

void UPGGuardianMovementComponent::UpdateFromCompressedFlags(const uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

float UPGGuardianMovementComponent::GetMaxSpeed() const
{
	const auto TempMaxSpeed = bWantsToSprint ? MaxSprintSpeed : MaxWalkSpeed;
	
	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return IsCrouching() ? MaxWalkSpeedCrouched : TempMaxSpeed;
	case MOVE_Falling:
		return TempMaxSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

void UPGGuardianMovementComponent::OnMovementUpdated(const float DeltaSeconds, const FVector& OldLocation,
                                                     const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	bToggledCrouch = bWantsToCrouchOld != bWantsToCrouch;
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
		bWantsToSprint = CMC->bWantsToSprint;
	}
}

void FSavedMove_Guardian::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	if (auto* CMC = Cast<UPGGuardianMovementComponent>(C->GetCharacterMovement()))
	{
		CMC->bWantsToSprint = bWantsToSprint;
	}
}

FNetworkPredictionData_Client_Guardian::FNetworkPredictionData_Client_Guardian(
	const UCharacterMovementComponent& ClientMovement)
	: FNetworkPredictionData_Client_Character(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_Guardian::AllocateNewMove()
{
	return MakeShared<FSavedMove_Guardian>();
}
