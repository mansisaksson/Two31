#pragma once

#include "Weapon.h"
#include "SMG.generated.h"

UCLASS()
class TWO31_API ASMG : public AWeapon
{
	GENERATED_BODY()
	
public:
	ASMG();

	virtual void FireShot(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* BulletTrail;

private:
	

};
