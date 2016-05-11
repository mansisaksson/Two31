#pragma once

#include "Weapons/Weapon.h"
#include "GaussRifle.generated.h"

UCLASS()
class TWO31_API AGaussRifle : public AWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleSpawnLocation;

public:
	AGaussRifle();

	virtual void FireShot(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* BulletTrail;

private:

};
