#pragma once

#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
#include "WeaponGlobals.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FImpactVisuals
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* WoodImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UMaterialInterface* WoodImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MetalImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UMaterialInterface* MetalImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* DefaultImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UMaterialInterface* DefaultImpactDecal;
};

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
	virtual void HolsterWeapon();
	virtual void SetPlayerAnimations(USkeletalMeshComponent* PlayerMesh);
	virtual void SetCultistAnimations(USkeletalMeshComponent* CultistMesh);
	virtual FString GetWeaponName();

	virtual void StartFire(FVector TowardsLocation);
	virtual void UpdateFire(FVector TowardsLocation);
	virtual void StopFire(FVector TowardsLocation);
	virtual void FireShot(FVector TowardsLocation);

	void Reload();
	void SetAmmoPool(int* ReserveAmmo);
	void FillClip();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetFireIsPressed() { return bFireIsPressed; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetCanFire() { return bCanFire; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetIsReadyToFire() { return bReadyToFire; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetIsReloading() { return bReload; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetClipSize() { return ClipSize; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoInClip() { return AmmoInClip; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	uint8 GetAmmoType() { return (uint8)AmmoType; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTotalReloadTime() { return TotalReloadTime; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTimeSinceReload() { return timeSinceReloadStart; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTimeSinceEquip() { return timeSinceEquip; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetIsEquiped() { return bIsEquiped; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	AActor* GetOwner();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponHit(FHitResult HitResult);
	virtual void OnWeaponHit_Implementation(FHitResult HitResult);
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginFire();
	void OnBeginFire_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateFire();
	void OnUpdateFire_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void OnStopFire();
	void OnStopFire_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* PlayerAnimationBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* CultistAnimationBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	FImpactVisuals ImpactVisuals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bAutoReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bFullAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float FullReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float FastReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RPM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float HeadshotMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float EquipTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float UnequipTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ImpulsePowah;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MinDecalSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxDecalSize;

	USkeletalMeshComponent* OwnerMesh;

	int AmmoInClip;
	int* AmmoPool;
	
	float TotalReloadTime;
	float timeSinceReloadStart;
	float timeSinceFire;
	float timeSinceEquip;
	float timeSinceUnequip;

	bool bIsEquiped;
	bool bEquip;
	bool bCanFire;
	bool bFireIsPressed;
	bool bFirstTimeEquiped;
	bool bReload;
	bool bReadyToFire;
	bool bHasFiredShot;
};
