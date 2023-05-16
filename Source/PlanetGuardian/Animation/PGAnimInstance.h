// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimationStateMachineLibrary.h"

#include "PGAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EPGCardinalDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EPGRootYawOffsetMode : uint8
{
	BlendOut,
	Hold,
	Accumulate
};

UCLASS()
class PLANETGUARDIAN_API UPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	bool bShowDebug { false };

public:
	void ToggleShowDebug();

protected:
	static FAnimationStateResultReference ConvertToAnimationState(const FAnimNodeReference& Node);

};
