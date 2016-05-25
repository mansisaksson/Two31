#pragma once

#include "Weapons/Weapon.h"
#include "GaussRifle.generated.h"

UCLASS()
class TWO31_API AGaussRifle : public AWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleSpawnLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* LaserDirection;

public:
	AGaussRifle();

	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void FireShot(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* TracerFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
	float HeatDissipationScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
	float HeatAccumulationScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shotgun)
	float MaxHeatAccumulation;

private:
	float HeatParam;
	UMaterialInstanceDynamic* MatInstance;
};
