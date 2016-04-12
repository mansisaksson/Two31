#pragma once

#include "Utilities/Weapon.h"
#include "RocketLauncher.generated.h"

UCLASS()
class TWO31_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()

public:
	ARocketLauncher();

	virtual void FireShot(FVector TowardsLocation) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimMontage* ArmFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TSubclassOf<class AProjectile> ProjectileClass;
};
