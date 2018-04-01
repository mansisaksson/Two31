// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Two31EditorTarget : TargetRules
{
	public Two31EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		
		ExtraModuleNames.AddRange( new string[] { "Two31" } );
	}
}
