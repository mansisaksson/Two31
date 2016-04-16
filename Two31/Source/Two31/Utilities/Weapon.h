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
	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalMesh, int* AmmoPool);
	virtual void SetPlayerAnimations(USkeletalMeshComponent* PlayerMesh);
	virtual void SetCultistAnimations(USkeletalMeshComponent* CultistMesh);

	virtual void StartFire(FVector TowardsLocation);
	virtual void UpdateFire(FVector TowardsLocation);
	virtual void StopFire(FVector TowardsLocation);
	virtual void FireShot(FVector TowardsLocation);

	void Reload();
	void SetAmmoPool(int* ReserveAmmo);
	void FillClip();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetIsFiring();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetClipSize();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoInClip();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	uint8 GetAmmoType();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTotalReloadTime();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTimeSinceReload();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	AActor* GetOwner();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* PlayerAnimationBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* CultistAnimationBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bAutoReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bFullAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RPM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage;
	
	USkeletalMeshComponent* OwnerMesh;

	int AmmoInClip;
	int* AmmoPool;

	float timeSinceReloadStart;
	float timeSinceFire;

	bool bCanFire;
	bool bIsFiring;
	bool bFirstTimeEquiped;

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponHit(FHitResult HitResult);
	void OnWeaponHit_Implementation(FHitResult HitResult);

private:
	bool bReload;
	bool bReadyToFire;

};
