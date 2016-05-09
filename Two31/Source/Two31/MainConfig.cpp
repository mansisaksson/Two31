
#include "Two31.h"
#include <sstream>
#include <iostream>
#include "MainConfig.h"

UMainConfig::UMainConfig()
{
	LoadConfig();
}

UMainConfig::~UMainConfig()
{
	
}

void UMainConfig::Save()
{
	SaveConfig();
}

void UMainConfig::Load()
{
	LoadConfig();
}

void UMainConfig::Apply()
{

	if(GraphicsResolution.IsEmpty()){
		GraphicsResolution = FString(TEXT("800x600"));
	}

	ExecCmd(FString::Printf(TEXT("fov %i"), GraphicsFOV));

	if(GraphicsFullscreen){
		ExecCmd(FString(TEXT("r.setRes " + GraphicsResolution + "f")));
	}else{
		ExecCmd(FString(TEXT("r.setRes " + GraphicsResolution + "w")));
	}

	ExecCmd(FString::Printf(TEXT("r.PostProcessAAQuality %i"), GraphicsAntiAliasing));
	ExecCmd(FString::Printf(TEXT("r.ViewDistanceScale %f"), GraphicsRenderDistanceScale));
	ExecCmd(FString::Printf(TEXT("sg.PostProcessQuality %i"), GraphicsPostProcessing));
	ExecCmd(FString::Printf(TEXT("sg.ShadowQuality %i"), GraphicsShadows));
	ExecCmd(FString::Printf(TEXT("sg.TextureQuality %i"), GraphicsTextures));
	ExecCmd(FString::Printf(TEXT("sg.EffectsQuality %i"), GraphicsEffects));

	if(GraphicsAntiAliasing){
		ExecCmd(FString(TEXT("r.VSync 1")));
	}else{
		ExecCmd(FString(TEXT("r.VSync 0")));
	}

}

void UMainConfig::ExecCmd(FString Command)
{
	GEngine->Exec(GetWorld(), Command.GetCharArray().GetData());
	Debug::LogOnScreen(Command);
}