// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#include "Modules/ModuleManager.h"

class FPGPlanetGuardianModule final : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{		
	}

	virtual void ShutdownModule() override
	{		
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FPGPlanetGuardianModule, PlanetGuardian, "PlanetGuardian");

