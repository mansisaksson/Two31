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

	void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Configuration")
	UMainConfig* GetConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
	bool bEnableProgMod;

private:
	UMainConfig* Config;

};
