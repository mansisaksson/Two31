#pragma once

#include "MainConfig.generated.h"

UCLASS(Config = Two31)
class TWO31_API UMainConfig : public UObject
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MainConfig")
	static UMainConfig* getInstance();

	UFUNCTION(BlueprintCallable, Category = "MainConfig")
	void save();

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	float testValue;

private:
	UMainConfig();
	~UMainConfig();

	static UMainConfig* Instance;

};