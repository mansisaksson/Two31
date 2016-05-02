#pragma once
#include "Pickup.h"
#include "../Weapons/Weapon.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TWO31_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:	
	AAmmoPickup();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmount();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	EAmmoType GetAmmoType();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 AmmoValue;
};
