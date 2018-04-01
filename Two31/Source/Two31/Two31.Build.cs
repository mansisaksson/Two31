// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Two31 : ModuleRules
{
	public Two31(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "InputCore", "AIModule", "ApexDestruction" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "Engine" });
	}
}
