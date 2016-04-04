#include "Two31.h"
#include "HealthPickup.h"

AHealthPickup::AHealthPickup()
	: APickup()
{
	HealthValue = 10;
}

float AHealthPickup::GetHealth()
{
	return HealthValue;
}