// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlanetGuardian : ModuleRules
{
	public PlanetGuardian(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
        });

		PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",
            "Niagara", 
            "GameplayCameras",
        });

        PrivateIncludePaths.Add("PlanetGuardian");

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
