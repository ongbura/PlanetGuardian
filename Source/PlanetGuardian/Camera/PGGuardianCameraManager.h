// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PGGuardianCameraManager.generated.h"

class APGGuardian;
class UPGSpringArmComponent;
class UCameraComponent;
class UPGGuardianMovementComponent;

UCLASS()
class PLANETGUARDIAN_API APGGuardianCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	TWeakObjectPtr<UPGGuardianMovementComponent> CMC;
	TWeakObjectPtr<UCameraComponent> Camera;
	TWeakObjectPtr<UPGSpringArmComponent> CameraBoom;

	bool bWasGuardianSprinting { false };

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> SprintingCameraShakeClass;

	UPROPERTY()
	UCameraShakeBase* SprintingCameraShake;

	UPROPERTY(EditDefaultsOnly)
	float CameraBlendSpeed { 8.f };

	UPROPERTY(EditDefaultsOnly)
	float FOVOnSprint { 70.f };

	float DefaultFOV { 0.f };

public:
	APGGuardianCameraManager();

	void InitializeObjects(const APGGuardian* Guardian);

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
