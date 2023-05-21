// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGuardianAnimationSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPGGuardianAnimInstanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turn In Place", meta=(UIMin="-180.0", UIMax="180.0"))
	FVector2D RootYawOffsetAngleClamp { -120.f, 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turn In Place", meta=(UIMin="-180.0", UIMax="180.0"))
	FVector2D RootYawOffsetAngleClampCrouched { -90.f, 80.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation Data", meta=(UIMin="0.0", UIMax="90.0"))
	float MaxLeanAmount { 20.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation Data", meta=(UIMin="-90.0", UIMax="0.0"))
	float MinLeanAmount { -20.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation Data", meta=(UIMin="0.0", UIMax="1.0"))
	float LeanAmountCoefficientOnSlow { 0.025f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation Data", meta=(UIMin="0.0", UIMax="1.0"))
	float LeanAmountCoefficientOnNormal { 0.0375f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Velocity Data")
	float CardinalDirectionDeadZone { 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pivot Data")
	float InitialLastPivotTime { 0.2f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functionality")
	bool bEnableFootPlacement { true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functionality")
	bool bEnableRootYawOffset { true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Curve")
	FName TurnYawWeightCurveName { "TurnYawWeight" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Curve")
	FName RemainingTurnYawCurveName { "RemainingTurnYaw" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Curve")
	FName DisableFootPlacementCurveName { "DisableLegIK" };
};
