// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGAbilityInputData.h"

UPGAbilityInputData::UPGAbilityInputData()
{
	AbilityInputSets.Reserve(MaxNumAbilityInputSet);
}

#if WITH_EDITOR
void UPGAbilityInputData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const auto PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UPGAbilityInputData, AbilityInputSets))
	{
		int32 Idx = 0;
		for (auto& Elem : AbilityInputSets)
		{
			Elem.InputID = ++Idx;
		}
	}
}
#endif
