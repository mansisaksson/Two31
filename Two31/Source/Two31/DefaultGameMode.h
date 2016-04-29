#pragma once

#include "MainConfig.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"


UCLASS()
class TWO31_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADefaultGameMode();

	UFUNCTION(BlueprintCallable, Category = "Configuration")
	UMainConfig* GetConfig();

private:
	UMainConfig* Config;

};
