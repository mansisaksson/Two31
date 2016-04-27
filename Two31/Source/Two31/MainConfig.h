
#pragma once

#include "MainConfig.generated.h"

UCLASS(Config=Two31)
class TWO31_API UMainConfig : public UObject
{

GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	static UMainConfig* getInstance()
	{
		return Instance;
	}

	UPROPERTY(Config)
	float testValue;

private:
	UMainConfig();
	~UMainConfig();

	static UMainConfig* Instance;

};
