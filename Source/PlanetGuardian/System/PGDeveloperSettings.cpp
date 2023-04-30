// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGDeveloperSettings.h"

void UPGDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPGDeveloperSettings, bAddEffectEmittersWhenRunOutOfPool))
	{
		if (!bAddEffectEmittersWhenRunOutOfPool)
		{
			NumNewEffectEmittersPerFrame = DEFAULT_NEW_EFFECT_EMITTERS;
		}
	}
}
