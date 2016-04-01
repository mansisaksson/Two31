#pragma once
#include "GameFramework/Actor.h"
#include "../Globals.h"
#include "Pickupable.generated.h"

UCLASS()
class TWO31_API APickupable : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* TriggerSphere;

public:	
	APickupable();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EItemType ItemType;
};
