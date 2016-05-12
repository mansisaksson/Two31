// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SubstanceCore : ModuleRules
	{
		public SubstanceCore(TargetInfo Target)
		{
			// internal defines
            Definitions.Add("WITH_SUBSTANCE=1");
			Definitions.Add("SUBSTANCE_PLATFORM_BLEND=1");

            if (Target.Platform == UnrealTargetPlatform.Win32 ||
                Target.Platform == UnrealTargetPlatform.Win64 ||
				Target.Platform == UnrealTargetPlatform.XboxOne)
            {
                Definitions.Add("AIR_USE_WIN32_SYNC=1");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac || 
		             Target.Platform == UnrealTargetPlatform.Linux)
            {
                Definitions.Add("AIR_USE_PTHREAD_SYNC=1");
            }
		
			PrivateIncludePaths.Add("SubstanceCore/Private");
			
            PublicDependencyModuleNames.AddRange(new string[] {
                    "AssetRegistry",
					"Core",
					"Core",
					"CoreUObject",
					"Engine",
					"RenderCore",
					"RHI",
					"ImageWrapper",
					"Settings",
					"SessionServices",
					"RHI"
				});

			if (UEBuildConfiguration.bBuildEditor == true)
			{
				PublicDependencyModuleNames.AddRange(new string[] {
					"UnrealEd",
					"AssetTools",
					"ContentBrowser",
                    "TargetPlatform"
                });
			}

            //Determine the root directory
            string ModuleCSFilename = RulesCompiler.GetModuleFilename(this.GetType().Name);
            string SubstanceBaseDir = Path.GetDirectoryName(ModuleCSFilename);
            string SubstanceLibPath = SubstanceBaseDir + "/lib/";

            //Include static lib
            if (Target.Platform == UnrealTargetPlatform.Linux)
		    {
			    SubstanceLibPath += "linux/release/";
                PublicAdditionalLibraries.Add(SubstanceLibPath + "libSubstance.a");
		    }
	        else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                SubstanceLibPath += "mac/release/";
                PublicAdditionalLibraries.Add(SubstanceLibPath + "libSubstance.a");
            }
            else if (Target.Platform == UnrealTargetPlatform.Win32)
            {
				SubstanceLibPath += GetWindowsLibPath(false) + "/release_md/";
				
                PublicAdditionalLibraries.Add(SubstanceLibPath + "Substance.lib");
            }
            else if (Target.Platform == UnrealTargetPlatform.Win64)
            {
				SubstanceLibPath += GetWindowsLibPath(true) + "/release_md/";
				
                PublicAdditionalLibraries.Add(SubstanceLibPath + "Substance.lib");
            }
            else
            {
                throw new BuildException("Platform not supported by Substance.");
            }
		}
		
		private string GetWindowsLibPath(bool b64Bit)
		{
			string basePath = "win32-msvc";
			
			switch (WindowsPlatform.Compiler)
			{
			case WindowsCompiler.VisualStudio2012:
					basePath += "2012";
					break;
			case WindowsCompiler.VisualStudio2013:
					basePath += "2013";
					break;
			case WindowsCompiler.VisualStudio2015:
					basePath += "2015";
					break;
			default:
					basePath += "UNKNOWN";
					break;
			}
			
			if (b64Bit)
			{
				basePath += "-64";
			}
			
			return basePath;
		}
	}
}
