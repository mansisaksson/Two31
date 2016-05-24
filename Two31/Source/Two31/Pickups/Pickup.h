#pragma once
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class TWO31_API APickup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* TriggerSphere;

public:	
	APickup();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	class ADefaultGameMode* DefaultGameMode;

	float BobSpeed;
	float BobHeight;
	float BobTimer;
	float RotSpeed;
};
