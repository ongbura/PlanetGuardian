// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimationStateMachineLibrary.h"
#include "SequenceEvaluatorLibrary.h"
#include "SequencePlayerLibrary.h"
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
	UPROPERTY(EditDefaultsOnly)
	bool bShowDebug { false };

public:
	void ToggleShowDebug();

protected:
	static FAnimationStateResultReference ConvertToAnimationState(const FAnimNodeReference& Node);

	static bool IsStateBlendingIn(const FAnimUpdateContext& Context, const FAnimationStateResultReference& Node);
	
	static bool IsStateBlendingOut(const FAnimUpdateContext& Context, const FAnimationStateResultReference& Node);

	static FSequencePlayerReference SetSequenceAsPlayer(const FAnimNodeReference& Node, UAnimSequence* Sequence);

	static FSequencePlayerReference SetSequenceWithInertialBlendingAsPlayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node, UAnimSequence* Sequence);

	static FSequencePlayerReference ConvertToSequencePlayer(const FAnimNodeReference& Node);
	
	static FSequenceEvaluatorReference SetSequenceAsEvaluator(const FAnimNodeReference& Node, UAnimSequence* Sequence);

	static FSequenceEvaluatorReference SetSequenceWithInertialBlendingAsEvaluator(const FAnimUpdateContext& Context, const FAnimNodeReference& Node, UAnimSequence* Sequence);

	static FSequenceEvaluatorReference ConvertToSequenceEvaluator(const FAnimNodeReference& Node);
};
