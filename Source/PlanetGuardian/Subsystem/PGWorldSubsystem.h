// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PGWorldSubsystem.generated.h"

class UPGAssetManager;

UCLASS()
class PLANETGUARDIAN_API UPGWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return false; }
};
