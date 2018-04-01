#include "ArmorPickup.h"
#include "Two31.h"

AArmorPickup::AArmorPickup()
	: APickup()
{
	ArmorValue = 10;
}

float AArmorPickup::GetArmor()
{
	return ArmorValue;
}