#include "Two31.h"
#include "ArmorPickup.h"

AArmorPickup::AArmorPickup()
	: APickup()
{
	ArmorValue = 10;
}

float AArmorPickup::GetArmor()
{
	return ArmorValue;
}