#pragma once
#include "Pickup.h"
#include "ArmorPickup.generated.h"

UCLASS()
class TWO31_API AArmorPickup : public APickup
{
	GENERATED_BODY()

public:	
	AArmorPickup();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetArmor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 ArmorValue;
};
