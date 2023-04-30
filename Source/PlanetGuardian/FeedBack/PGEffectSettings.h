// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGEffectSettings.generated.h"

USTRUCT(BlueprintType)
struct FPGEffectSettings_VFX
{
	GENERATED_BODY()
	
	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
};

USTRUCT(BlueprintType)
struct FPGEffectSettings_SFX
{
	GENERATED_BODY()
	
	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float VolumeMultiplier = 1.0f;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float PitchMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FPGEffectSettings_Trace
{
	GENERATED_BODY()

	// Trace Channel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	// Vector offset from Effect Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	FVector EndTraceLocationOffset = FVector::ZeroVector;

	// Ignore this Actor when getting trace result
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	bool bIgnoreActor = true;
};
