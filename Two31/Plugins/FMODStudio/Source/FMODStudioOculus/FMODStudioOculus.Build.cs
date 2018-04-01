// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2016.

namespace UnrealBuildTool.Rules
{
	public class FMODStudioOculus : ModuleRules
	{
		public FMODStudioOculus(ReadOnlyTargetRules Target) : base(Target)
		{
			// For some reason this flag blows out the size of the compiled lib from under 50MB to over 100MB!
			//bFasterWithoutUnity = true;

			PublicIncludePaths.AddRange(
				new string[] {
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"FMODStudioOcculus/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"FMODStudio"
				}
				);

			// ModuleDirectory points to FMODStudio\source\FMODStudio, need to get back to lib
			string BasePath = System.IO.Path.Combine(ModuleDirectory, "../../Binaries", Target.Platform.ToString());

			switch (Target.Platform)
			{
				case UnrealTargetPlatform.Win32:
					PublicAdditionalLibraries.Add(System.IO.Path.Combine(BasePath, "ovrfmod.lib"));
					PublicDelayLoadDLLs.Add("ovrfmod.dll");
					PublicDefinitions.Add("FMOD_OSP_SUPPORTED=1");
					break;
				case UnrealTargetPlatform.Win64:
					PublicAdditionalLibraries.Add(System.IO.Path.Combine(BasePath, "ovrfmod.lib"));
					PublicDelayLoadDLLs.Add("ovrfmod.dll");
					PublicDefinitions.Add("FMOD_OSP_SUPPORTED=1");
					break;
				default:
					break;
			}

			if (Target.Type == TargetType.Editor)
			{
				PublicDependencyModuleNames.AddRange(
					new string[]
					{
						"UnrealEd",
						"Slate",
						"SlateCore",
						"Settings",
					}
					);
			}
		}
	}
}