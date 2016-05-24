#pragma once

#include "Pickup.h"
#include "ItemPickup.generated.h"

UCLASS()
class TWO31_API AItemPickup : public APickup
{
	GENERATED_BODY()

public:
	AItemPickup();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetItemID() { return ID; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	FString GetItemName() { return Name; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString Name;

};
