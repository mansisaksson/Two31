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

	UFUNCTION(BlueprintCallable, Category = "MainConfig")
	void Load();

	/*
		Configuration Values
	*/

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	FString GraphicsResolution;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	bool GraphicsFullscreen;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsFOV;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsAntiAliasing;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsPostProcessing;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsShadows;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsTextures;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsEffects;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	float GraphicsRenderDistanceScale;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	bool GraphicsVSync;
	
};