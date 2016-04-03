#pragma once
#include "Object.h"
#include "Globals.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8 
{
	SMG				UMETA(DisplayName = "SMG"),
	Explosive		UMETA(DisplayName = "Explosive"),
	Plasma			UMETA(DisplayName = "Plasma")
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
