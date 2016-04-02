#pragma once

#include "Utilities/Weapon.h"
#include "RocketLauncher.generated.h"

UCLASS()
class TWO31_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()

public:
	ARocketLauncher();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartFire(FVector TowardsLocation) override;
	virtual void UpdateFire(float DeltaSeconds, FVector TowardsLocation) override;
	virtual void StopFire(FVector TowardsLocation) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimMontage* ArmFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AProjectile> ProjectileClass;
};
