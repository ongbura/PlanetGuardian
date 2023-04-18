// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGInputConfig.h"

const UInputAction* UPGInputConfig::FindInputActionForTag(const FGameplayTag& Tag) const
{
	for (const auto& [InputAction, InputTag] : TaggedInputActions)
	{
		if (InputAction && InputTag == Tag)
		{
			return InputAction;
		}
	}

	return nullptr;
}
