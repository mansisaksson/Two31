#pragma once
#include "Pickup.h"
#include "HealthPickup.generated.h"

UCLASS()
class TWO31_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:	
	AHealthPickup();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 HealthValue;
};
