#include "Two31.h"
#include "Characters/PlayerHUD.h"
#include "DefaultGameMode.h"

ADefaultGameMode::ADefaultGameMode() : Super()
{
	Config = NewObject<UMainConfig>(UMainConfig::StaticClass());
}

UMainConfig* ADefaultGameMode::GetConfig()
{
	return Config;
}