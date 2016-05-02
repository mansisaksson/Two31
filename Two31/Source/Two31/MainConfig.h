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

	UENUM(BlueprintType)
	enum class EQuality : uint8
	{
		LOW		UMETA(DisplayName = "Low"),
		NORMAL	UMETA(DisplayName = "Normal"),
		HIGH	UMETA(DisplayName = "High"),
		EPIC	UMETA(DisplayName = "Epic")
	};

	/*
		Configuration Values
	*/

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	EQuality GraphicsAntiAliasing;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	EQuality GraphicsPostProcessing;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	EQuality GraphicsShadows;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	EQuality GraphicsTextures;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	EQuality GraphicsEffects;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsFullscreen;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	int32 GraphicsFOV;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MainConfig")
	FString GraphicsResolution;
	
};