#include "Two31.h"
#include "../Weapon.h"
#include "WeaponPickup.h"

AWeaponPickup::AWeaponPickup()
	: APickup()
{
	StartingAmmo = 100;
}

TSubclassOf<AWeapon> AWeaponPickup::GetWeapon()
{
	return Weapon;
}

int AWeaponPickup::GetStartingAmmo()
{
	return StartingAmmo;
}