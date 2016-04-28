
#include "Two31.h"
#include "MainConfig.h"

UMainConfig::UMainConfig()
{
	LoadConfig();
}

UMainConfig::~UMainConfig()
{
	SaveConfig();
}

void UMainConfig::save()
{
	SaveConfig();
}