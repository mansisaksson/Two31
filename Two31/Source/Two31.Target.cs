// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Two31Target : TargetRules
{
	public Two31Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		ExtraModuleNames.AddRange( new string[] { "Two31" } );
	}
}
