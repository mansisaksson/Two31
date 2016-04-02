#pragma once
#include "Object.h"
#include "Globals.generated.h"

#define COLLISION_BEAMTRACE		ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EItemType : uint8 
{
	SMG				UMETA(DisplayName = "SMG"),
	RocketLauncher	UMETA(DisplayName = "RocketLauncher"),
	Shotgun			UMETA(DisplayName = "Shotgun"),
	Pistol			UMETA(DisplayName = "Pistol"),
	Armor			UMETA(DisplayName = "Armor"),
	HealthPack		UMETA(DisplayName = "HealthPack"),
	None			UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 var1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float var2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float var3;
};

UCLASS()
class TWO31_API UGlobals : public UObject
{
	GENERATED_BODY()


};
