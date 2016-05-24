#pragma once
#include "Pickup.h"
#include "WeaponPickup.generated.h"

UCLASS()
class TWO31_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

public:	
	AWeaponPickup();

	TSubclassOf<class AWeapon> GetWeapon();
	int GetStartingAmmo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<class AWeapon> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 StartingAmmo;
};
