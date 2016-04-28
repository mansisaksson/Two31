
#include "Two31.h"
#include <sstream>
#include <iostream>
#include "MainConfig.h"

UMainConfig* UMainConfig::Instance = NULL;

UMainConfig::UMainConfig()
{
	
}

UMainConfig::~UMainConfig()
{
	
}

UMainConfig* UMainConfig::getInstance()
{
	if (!Instance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loading Config!"));
		Instance = NewObject<UMainConfig>(UMainConfig::StaticClass());
		Instance->LoadConfig();
	}
	return Instance;
}

void UMainConfig::save()
{
	std::ostringstream ss;
	ss << testValue;
	FString str = UTF8_TO_TCHAR(ss.str().c_str());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Saving Config!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, str);
	SaveConfig();
}