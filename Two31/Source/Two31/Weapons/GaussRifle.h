// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/Weapon.h"
#include "GaussRifle.generated.h"

/**
 * 
 */
UCLASS()
class TWO31_API AGaussRifle : public AWeapon
{
	GENERATED_BODY()
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
