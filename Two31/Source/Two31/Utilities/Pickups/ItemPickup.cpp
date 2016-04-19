// Fill out your copyright notice in the Description page of Project Settings.

#include "Two31.h"
#include "ItemPickup.h"

AItemPickup::AItemPickup()
	: APickup()
{
	ItemType = EItemType::BlueKey;
}

EItemType AItemPickup::GetItemType()
{
	return ItemType;
}


