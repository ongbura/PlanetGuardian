// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianAnimInstanceProxy.h"
#include "Character/Guardian/PGGuardian.h"
#include "PGNativeGameplayTags.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"

FPGGuardianAnimInstanceProxy::FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance)
	: Super(InAnimInstance)
{
}

void FPGGuardianAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);

	const auto* Pawn = InAnimInstance->TryGetPawnOwner();
	if (Pawn == nullptr)
	{
		return;
	}

	const auto ThisWorldType = Pawn->GetWorld()->WorldType;
	if (ThisWorldType == EWorldType::Game || ThisWorldType == EWorldType::PIE || ThisWorldType == EWorldType::GameRPC)
	{
		bShouldUpdate = true;
	}
	else
	{
		bShouldUpdate = false;
		return;
	}

	const auto* Guardian = Cast<APGGuardian>(InAnimInstance->TryGetPawnOwner());
	if (!ensure(Guardian))
	{
		return;
	}

	WorldLocation = Guardian->GetActorLocation();
	WorldRotation = Guardian->GetActorRotation();
	WorldVelocity = Guardian->GetVelocity();
	Pitch = Guardian->GetBaseAimRotation().Pitch;

	auto* CMC = Cast<UPGGuardianMovementComponent>(Guardian->GetCharacterMovement());
	check(CMC);

	WorldAcceleration = CMC->GetCurrentAcceleration();
	bIsOnGround = CMC->IsMovingOnGround();
	bIsCrouching = CMC->IsCrouching();

	if (CMC->MovementMode == MOVE_Falling)
	{
		if (WorldVelocity.Z > 0.f)
		{
			bIsJumping = true;
		}
		else
		{
			bIsFalling = true;
		}
	}

	GravityZ = CMC->GetGravityZ();
	DistanceFromGround = CMC->GetJumpFallData().DistanceFromGround;

	if (const auto* AbilitySystem = Guardian->GetPGAbilitySystemComponent())
	{
		bIsOnADS = AbilitySystem->HasMatchingGameplayTag(GNativeTags.State_Guardian_IsOnADS);
		bIsFiring = AbilitySystem->HasMatchingGameplayTag(GNativeTags.State_Guardian_IsFiring);
		bIsReloading = AbilitySystem->HasMatchingGameplayTag(GNativeTags.State_Guardian_IsReloading);
	}
	else
	{
		bIsOnADS = false;
		bIsFiring = false;
		bIsReloading = false;
	}	
}