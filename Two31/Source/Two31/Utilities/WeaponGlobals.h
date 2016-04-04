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

UCLASS()
class TWO31_API UWeaponGlobals : public UObject
{
	GENERATED_BODY()

};
