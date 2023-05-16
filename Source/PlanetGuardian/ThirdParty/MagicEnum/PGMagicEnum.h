// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "magic_enum.hpp"

/**
 * 
 */
class FPGMagicEnum
{
public:
	template <typename EnumType>
	static FString GetEnumString(const EnumType Value)
	{
		const std::string_view ViewName = magic_enum::enum_name(Value);
		return { static_cast<int32>(ViewName.size()),  ViewName.data() };
	}
};
