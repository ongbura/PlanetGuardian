// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#include "PGInputConfig.h"

const UInputAction* UPGInputConfig::FindInputActionForTag(const FGameplayTag& Tag) const
{
	if (const auto* Found = InputActions.Find({ nullptr, Tag }))
	{
		return Found->InputAction;
	}

	return nullptr;
}
