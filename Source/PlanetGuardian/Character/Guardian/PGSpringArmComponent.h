// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PGSpringArmComponent.generated.h"


class UPGGuardianMovementComponent;

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float CrouchBlendSpeed { 10.f };

	TWeakObjectPtr<UPGGuardianMovementComponent> CMC;

	float DefaultCapsuleHalfHeight { 0.f };

	float CrouchedCapsuleHalfHeight { 0.f };

	bool CrouchBlendFinished { false };

	float InitialZLocation { 0.f };

public:
	UPGSpringArmComponent();

protected:
	virtual void InitializeComponent() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
