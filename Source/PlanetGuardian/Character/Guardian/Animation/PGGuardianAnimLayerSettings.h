// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGuardianAnimLayerSettings.generated.h"

USTRUCT(BlueprintType)
struct FPGGuardianAnimLayerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Distance Matching")
	FVector2D PlayRateClamp { 0.75f, 1.25f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stride Warping")
	float StrideWarpingBlendInDurationScaled { 0.2f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stride Warping")
	float StrideWarpingBlendInStartOffset { 0.15f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Play State")
	float RaiseWeaponAfterFiringDuration { 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Orientation Warping")
	float OrientationWarpingSpeed { 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Orientation Warping")
	bool bEnableOrientationWarping { true };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Play State")
	bool bRaiseWeaponAfterFiringWhenCrouched { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pose Override")
	bool bEnableLeftHandPoseOverride { false };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skel Control Data")
	bool bDisableHandIK { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Curve")
	FName DistanceCurveName { "Distance" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Curve")
	FName JumpDistanceCurveName { "GroundDistance" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Curve")
	FName ApplyHipFireOverridePoseCurveName { "applyHipfireOverridePose" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Curve")
	FName DisableRightHandIKCurveName { "DisableRHandIK" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Curve")
	FName DisableLeftHandIKCurveName { "DisableLHandIK" };

	
};