#include "Two31.h"
#include "AmmoPickup.h"

AAmmoPickup::AAmmoPickup()
	: APickup()
{
	AmmoType = EAmmoType::BulletAmmo;
	AmmoValue = 100;
}

int32 AAmmoPickup::GetAmount()
{
	return AmmoValue;
}

EAmmoType AAmmoPickup::GetAmmoType()
{
	return AmmoType;
}