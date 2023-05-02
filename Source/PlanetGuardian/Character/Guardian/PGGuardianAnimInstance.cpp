// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuardianAnimInstance.h"
#include "Camera/PGCameraShake.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		if (FVector::DotProduct(RightVector, NormalizedVel) < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

FPGGuardianAnimInstanceProxy::FPGGuardianAnimInstanceProxy(UAnimInstance* InAnimInstance)
	: Super(InAnimInstance)
{
	AnimInstance = CastChecked<UPGGuardianAnimInstance>(InAnimInstance);
}

void FPGGuardianAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);

	if (const auto* const CharacterOwner = Cast<ACharacter>(InAnimInstance->TryGetPawnOwner()))
	{
		CapsuleComp = CastChecked<UCapsuleComponent>(CharacterOwner->GetRootComponent());
		MovementComp = CharacterOwner->GetCharacterMovement();
	}
}

void FPGGuardianAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);
}

void FPGGuardianAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);

	auto* Data = AnimInstance->BackAnimData;

	if (!MovementComp.IsValid() || !CapsuleComp.IsValid())
	{
		return;
	}

	const FVector GroundVelocity{ MovementComp->Velocity.X, MovementComp->Velocity.Y, 0.f };

	Data->GroundSpeed = GroundVelocity.Length();

	constexpr static float MovingTolerance{ 5.f };
	Data->bIsMoving = Data->GroundSpeed > MovingTolerance ? true : false;

	Data->bIsInAir = MovementComp->IsFalling();
	Data->bIsAccelerating = MovementComp->GetCurrentAcceleration().IsNearlyZero() ? false : true;
	Data->bIsCrouching = MovementComp->IsCrouching();
	Data->Direction = CalculateDirection(MovementComp->Velocity, CapsuleComp->GetComponentRotation());

	CalculateLeanAmount(Data, CapsuleComp->GetComponentRotation(), DeltaSeconds);

	// TODO: Update bIsHovering, bIsInactive, InteractWorldLocation
}

void FPGGuardianAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	FAnimInstanceProxy::PostUpdate(InAnimInstance);
	
	auto* FrontAnimData = AnimInstance->AnimData;
	AnimInstance->AnimData = AnimInstance->BackAnimData;
	AnimInstance->BackAnimData = FrontAnimData;
}

void FPGGuardianAnimInstanceProxy::CalculateLeanAmount(UPGGuardianAnimData* Data, const FRotator& InCurrentRotation, const float DeltaSeconds)
{
	const auto DeltaRotation = (InCurrentRotation - Data->PreviousRotation).GetNormalized();
	Data->LeanAmount = UKismetMathLibrary::FInterpTo(Data->LeanAmount, DeltaRotation.Yaw, DeltaSeconds, 2.f);

	Data->PreviousRotation = InCurrentRotation;
}

void UPGGuardianAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	bUseMultiThreadedAnimationUpdate = true;

	AnimData = NewObject<UPGGuardianAnimData>();
	BackAnimData = NewObject<UPGGuardianAnimData>();
}

FAnimInstanceProxy* UPGGuardianAnimInstance::CreateAnimInstanceProxy()
{
	return new FPGGuardianAnimInstanceProxy(this);
}

void UPGGuardianAnimInstance::AnimNotify_ShakeCameraOnLand()
{
	if (!CameraShake->IsValidLowLevel())
	{
		return;
	}

	const auto* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);	
	const auto CameraLocation = CameraManager->GetCameraLocation();

	CameraManager->PlayWorldCameraShake(GetWorld(), CameraShake, CameraLocation, 400.f, 800.f, 1.f);
}
