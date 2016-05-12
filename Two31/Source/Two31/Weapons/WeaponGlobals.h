#pragma once
#include "Object.h"
#include "WeaponGlobals.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	BulletAmmo		UMETA(DisplayName = "BulletAmmo"),
	ShotgunAmmo		UMETA(DisplayName = "ShotGunAmmo"),
	ExplosiveAmmo	UMETA(DisplayName = "ExplosiveAmmo"),
	PlasmaAmmo		UMETA(DisplayName = "PlasmaAmmo")
};

USTRUCT(BlueprintType)
struct FAmmo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPool = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo = 200;
};

USTRUCT(BlueprintType)
struct FImpactDecals
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UMaterialInterface* Decal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	FVector Size = FVector(10.f, 10.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float AddedRandXYSize = 10.f;
};

USTRUCT(BlueprintType)
struct FImpactVisual
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	FString PhysicsMatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	TArray<FImpactDecals> ImpactDecals;
};

UCLASS()
class TWO31_API UWeaponGlobals : public UObject
{
	GENERATED_BODY()

};
