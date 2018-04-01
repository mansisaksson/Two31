#include "HealthPickup.h"
#include "Two31.h"

AHealthPickup::AHealthPickup()
	: APickup()
{
	HealthValue = 10;
}

float AHealthPickup::GetHealth()
{
	return HealthValue;
}