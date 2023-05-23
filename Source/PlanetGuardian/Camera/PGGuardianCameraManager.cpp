// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Character/Guardian/PGSpringArmComponent.h"
#include "Character/Guardian/PGGuardian.h"
#include "Character/Guardian/PGGuardianMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

APGGuardianCameraManager::APGGuardianCameraManager()
{
}

void APGGuardianCameraManager::InitializeObjects(const APGGuardian* Guardian)
{
	if (Guardian == nullptr)
	{
		return;
	}
	
	CMC = Cast<UPGGuardianMovementComponent>(Guardian->GetCharacterMovement());
		
	Camera = Guardian->GetFollowCamera();
	DefaultFOV = Camera->FieldOfView;

	CameraBoom = Guardian->GetCameraBoom();
}

void APGGuardianCameraManager::UpdateViewTarget(FTViewTarget& OutVT, const float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (!CMC.IsValid() || !Camera.IsValid() || !CameraBoom.IsValid())
	{
		return;
	}

	if (CMC->IsSprinting())
	{
		check(!FMath::IsNearlyZero(FOVOnSprint));
		
		Camera->FieldOfView = UKismetMathLibrary::FInterpTo(Camera->FieldOfView, FOVOnSprint, DeltaTime, CameraBlendSpeed);
		
		if (!bWasGuardianSprinting)
		{
			SprintingCameraShake = StartCameraShake(SprintingCameraShakeClass);
		}
	}
	else
	{
		check(!FMath::IsNearlyZero(DefaultFOV));
		
		Camera->FieldOfView = UKismetMathLibrary::FInterpTo(Camera->FieldOfView, DefaultFOV, DeltaTime, CameraBlendSpeed);
		
		if (bWasGuardianSprinting)
		{
			StopCameraShake(SprintingCameraShake, false);
		}
	}

	bWasGuardianSprinting = CMC->IsSprinting();
}
