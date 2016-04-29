#pragma once

#include "MainConfig.generated.h"

UCLASS(Config = Two31)
class TWO31_API UMainConfig : public UObject
{

	GENERATED_BODY()

public:

	UMainConfig();
	~UMainConfig();

	UFUNCTION(BlueprintCallable, Category = "MainConfig")
	void Save();

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	float testValue;
	
};