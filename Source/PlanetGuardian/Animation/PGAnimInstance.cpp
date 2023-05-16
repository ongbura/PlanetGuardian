// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAnimInstance.h"

#include "AnimationStateMachineLibrary.h"

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
