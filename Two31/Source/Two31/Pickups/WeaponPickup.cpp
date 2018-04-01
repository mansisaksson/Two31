#include "WeaponPickup.h"
#include "Two31.h"
#include "../Weapons/Weapon.h"

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