#pragma once

#include "Utilities/Weapon.h"
#include "Shotgun.generated.h"


UCLASS()
class TWO31_API AShotgun : public AWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleFlashLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* Lazer;

public:
	AShotgun();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void FireShot(FVector TowardsLocation) override;

protected:
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
