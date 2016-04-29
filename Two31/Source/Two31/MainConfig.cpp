
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