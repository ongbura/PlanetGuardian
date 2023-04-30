// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectSetData.h"

FPrimaryAssetId UPGEffectSetData::GetPrimaryAssetId() const
{
	return { GetPrimaryAssetType(), GetFName() };
}
