#include "Two31.h"
#include "Characters/PlayerHUD.h"
#include "DefaultGameMode.h"

ADefaultGameMode::ADefaultGameMode() : Super()
{
	Config = NewObject<UMainConfig>(UMainConfig::StaticClass());
	Config->AddToRoot();
	bEnableProgMod = false;
}

void ADefaultGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (bEnableProgMod) {
		GetConfig()->GameplayProggMode = true;
	}
}

UMainConfig* ADefaultGameMode::GetConfig()
{
	return Config;
}