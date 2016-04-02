#pragma once

#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
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
	virtual void SetArmAnimations(USkeletalMeshComponent* ArmMesh);
	virtual void StartFire(FVector TowardsLocation);
	virtual void UpdateFire(float DeltaSeconds, FVector TowardsLocation);
	virtual void StopFire(FVector TowardsLocation);

	bool GetIsFiring();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimBlueprintGeneratedClass* ArmAnimationBlueprint;

	USkeletalMeshComponent* ArmMesh;

private:
	bool bIsFiring;
};
