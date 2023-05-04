// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGDeveloperSettings.h"

#if WITH_EDITOR
void UPGDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const auto PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UPGDeveloperSettings, DefaultAbilities))
	{
		int32 Idx = 0;
		for (auto& Elem : DefaultAbilities)
		{
			Elem.InputID = ++Idx;
		}
	}
}
#endif
