#include "Two31.h"
#include "AmmoPickup.h"

AAmmoPickup::AAmmoPickup()
	: APickup()
{
	AmmoType = EAmmoType::SMG;
	AmmoValue = 100;
}