#pragma once

#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
#include "WeaponGlobals.h"
#include "Weapon.generated.h"

UCLASS()
class TWO31_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Visual)
	USceneComponent* BulletSpawnLocation;

public:	
	AWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;	

	/* These are all called from the PlayerCharacter */
	virtual void EquipWeapon(USkeletalMeshComponent* ArmMesh, int* AmmoPool);
	virtual void SetArmAnimations(USkeletalMeshComponent* ArmMesh);
	virtual void StartFire(FVector TowardsLocation);
	virtual void UpdateFire(float DeltaSeconds, FVector TowardsLocation);
	virtual void StopFire(FVector TowardsLocation);

	void SetAmmoPool(int* ReserveAmmo);

	bool GetIsFiring();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetClipSize();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoInClip();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	uint8 GetAmmoType();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* ArmAnimationBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EAmmoType AmmoType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 ClipSize;
	
	USkeletalMeshComponent* ArmMesh;

	int AmmoInClip;
	int* AmmoPool;

private:
	bool bIsFiring;
};
