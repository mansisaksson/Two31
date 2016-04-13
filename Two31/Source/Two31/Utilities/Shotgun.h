// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utilities/Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class TWO31_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AShotgun();

	virtual void FireShot(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		UAnimMontage* ArmFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		UParticleSystem* MuzzeFlash;

	void OnWeaponHit(FHitResult HitResult) override;

private:

};
