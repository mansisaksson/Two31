#pragma once
#include "Pickup.h"
#include "../Weapon.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TWO31_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:	
	AAmmoPickup();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 AmmoValue;
};
