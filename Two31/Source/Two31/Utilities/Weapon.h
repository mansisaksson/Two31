#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TWO31_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Visual)
	USceneComponent* BulletSpawnLocation;

public:	
	AWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;	

	virtual void StartFire(FVector TowardsLocation);
	// Is called from the PlayerCharacter
	virtual void UpdateFire(FVector TowardsLocation);
	virtual void EndFire(FVector TowardsLocation);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;

private:
	bool bIsFiring;
};
