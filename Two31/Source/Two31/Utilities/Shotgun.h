// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utilities/Weapon.h"
#include "Shotgun.generated.h"


UCLASS()
class TWO31_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AShotgun();

	virtual void FireShot(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
		int32 NumberOfShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
		float RadiusMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
		float RadiusMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
		float Distance;

private:

};
