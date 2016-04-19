// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utilities/Pickups/Pickup.h"
#include "ItemPickup.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	BlueKey		UMETA(DisplayName = "BlueKey"),
	RedKey		UMETA(DisplayName = "RedKey"),
	GreenKey	UMETA(DisplayName = "GreenKey")
};

UCLASS()
class TWO31_API AItemPickup : public APickup
{
	GENERATED_BODY()

public:
	AItemPickup();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	EItemType GetItemType();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	EItemType ItemType;
	
	
};
