
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
	ExecCmd(FString::Printf(TEXT("fov %i"), GraphicsFOV));
}

void UMainConfig::ExecCmd(FString Command)
{
	GetWorld()->Exec(GetWorld(), Command.GetCharArray().GetData());
}