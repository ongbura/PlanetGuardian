// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGEffectBundle.h"
#include "NiagaraSystem.h"
#include "System/PGAssetManager.h"

FSoftObjectPath UPGEffectBundle::GetSoftVisualFXPath() const
{
	return SoftVisualFX.ToSoftObjectPath();
}

UNiagaraSystem* UPGEffectBundle::GetVisualFX() const
{
	return SoftVisualFX.Get();
}

FSoftObjectPath UPGEffectBundle::GetSoftSoundFXPath() const
{
	return SoftSoundFX.ToSoftObjectPath();
}

USoundBase* UPGEffectBundle::GetSoundFX() const
{
	return SoftSoundFX.Get();
}

FPrimaryAssetId UPGEffectBundle::GetPrimaryAssetId() const
{
	return { UPGAssetManager::EffectBundleType, GetFName() };
}
