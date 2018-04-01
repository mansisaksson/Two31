#include "MainConfig.h"
#include "Two31.h"
#include <sstream>
#include <iostream>
#include "DefaultGameMode.h"

UMainConfig::UMainConfig()
{
	
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
	if (!FirstTime) {
		GraphicsResolution = FString(TEXT("1920x1080"));
		GraphicsFullscreen = true;
		GraphicsFOV = 90;
		GraphicsAntiAliasing = 6;
		GraphicsPostProcessing = 3;
		GraphicsShadows = 3;
		GraphicsTextures = 3;
		GraphicsEffects = 3;
		GraphicsRenderDistanceScale = 1.0f;
		GraphicsVSync = true;
		AudioMaster = 1.0f;
		AudioMusic = 1.0f;
		AudioDialog = 1.0f;
		AudioEnviroment = 1.0f;
		GameplaySubtitles = false;
		GameplayHints = true;
		GameplayProggMode = false;
		MouseSensitivity = 0.07f;
		//MostProcentOfEnemiesKilled = 0;
		//MostProcentOfSecretsFound = 0;
		//MostAmountOfBulletsFired = 0;
		//LeastAmountOfBulletsFired = 0;
		//MostAmountOfDamageTaken = 0;
		//LeastAmountOfDamageTaken = 0;
		//MostAmountOfHealthLost = 0;
		//LeastAmountOfHealthLost = 0;
		//MostAmountOfArmorLost = 0;
		//LeastAmountOfArmorLost = 0;
		Save();
	}
}

void UMainConfig::Apply()
{

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

	//GetWorld()->GetFirstPlayerController()->PlayerInput->SetMouseSensitivity(MouseSensitivity);

}

void UMainConfig::ExecCmd(FString Command)
{
	GEngine->Exec(GetWorld(), Command.GetCharArray().GetData());
	//Debug::LogOnScreen(Command);
}