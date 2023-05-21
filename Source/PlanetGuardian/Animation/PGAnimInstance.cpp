// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAnimInstance.h"

void UPGAnimInstance::ToggleShowDebug()
{
	bShowDebug = !bShowDebug;
}

FAnimationStateResultReference UPGAnimInstance::ConvertToAnimationState(const FAnimNodeReference& Node)
{
	FAnimationStateResultReference Result;
	bool bConverted;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResultPure(Node, Result, bConverted);
	check(bConverted);

	return Result;
}

bool UPGAnimInstance::IsStateBlendingIn(const FAnimUpdateContext& Context, const FAnimationStateResultReference& Node)
{
	return UAnimationStateMachineLibrary::IsStateBlendingIn(Context, Node);
}

bool UPGAnimInstance::IsStateBlendingOut(const FAnimUpdateContext& Context,
                                         const FAnimationStateResultReference& Node)
{
	return UAnimationStateMachineLibrary::IsStateBlendingOut(Context, Node);
}

FSequencePlayerReference UPGAnimInstance::SetSequenceAsPlayer(const FAnimNodeReference& Node, UAnimSequence* Sequence)
{
	FSequencePlayerReference SequencePlayer;
	bool bConverted;

	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bConverted);
	check(bConverted);

	USequencePlayerLibrary::SetSequence(SequencePlayer, Sequence);
	return SequencePlayer;
}

FSequencePlayerReference UPGAnimInstance::SetSequenceWithInertialBlendingAsPlayer(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node, UAnimSequence* Sequence)
{
	FSequencePlayerReference SequencePlayer;
	bool bConverted;

	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bConverted);
	check(bConverted);

	USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayer, Sequence);
	return SequencePlayer;
}

FSequencePlayerReference UPGAnimInstance::ConvertToSequencePlayer(const FAnimNodeReference& Node)
{
	FSequencePlayerReference SequencePlayer;
	bool bConverted;

	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bConverted);
	check(bConverted);

	return SequencePlayer;
}

FSequenceEvaluatorReference UPGAnimInstance::SetSequenceAsEvaluator(const FAnimNodeReference& Node, UAnimSequence* Sequence)
{
	FSequenceEvaluatorReference SequenceEvaluator;
	bool bConverted;

	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bConverted);
	check(bConverted);

	USequenceEvaluatorLibrary::SetSequence(SequenceEvaluator, Sequence);
	return SequenceEvaluator;
}

FSequenceEvaluatorReference UPGAnimInstance::SetSequenceWithInertialBlendingAsEvaluator(const FAnimUpdateContext& Context,
	const FAnimNodeReference& Node, UAnimSequence* Sequence)
{
	FSequenceEvaluatorReference SequenceEvaluator;
	bool bConverted;

	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bConverted);
	check(bConverted);

	USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, SequenceEvaluator, Sequence);
	return SequenceEvaluator;
}

FSequenceEvaluatorReference UPGAnimInstance::ConvertToSequenceEvaluator(const FAnimNodeReference& Node)
{
	FSequenceEvaluatorReference SequenceEvaluator;
	bool bConverted;

	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bConverted);
	check(bConverted);

	return SequenceEvaluator;
}
