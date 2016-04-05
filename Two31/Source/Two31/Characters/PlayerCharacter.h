#pragma once

#include "GameFramework/Character.h"
#include "../Utilities/WeaponGlobals.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TWO31_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPArmMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCamera;


public:
	APlayerCharacter();

	FORCEINLINE USkeletalMeshComponent* GetFPMeshArmMesh() const { return FPArmMesh; }
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FPCamera; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable, Category = GetFunction)
	bool PickupHealthPack(AHealthPickup* Healthpack);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetHealthPacks();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetArmor();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxArmor();
	UFUNCTION(BlueprintCallable,Category = GetFunction)
	bool ChangeArmor(float pChange);

	/* Ta bort, Kan använda GetWeapon() Istället */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetClipSize();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoInClip();
	/**/

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoPool();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetMaxAmmo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	class AWeapon* GetCurrentWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool EquipWeapon(TSubclassOf<class AWeapon> Weapon);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void OnFire();
	void OnReleaseFire();
	void OnReload();

	void SelectWeaponSlot(int index);
	void SelectWeaponSlot1();
	void SelectWeaponSlot2();
	void SelectWeaponSlot3();
	void SelectWeaponSlot4();

	void MoveForward(float Val);
	void MoveSideways(float Val);

	void StartSprint();
	void StopSprint();

	void NextWeapon();
	void PreviousWeapon();
	int GetIndex();

	void UseHealthPack();
	void TakeDamageTest();

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;
	/* Sprint speed = max walk speed, walk speed = max walk speed / muliplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float SprintMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bCanJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxAmountOfHealthPacks;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon1;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon2;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon3;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FAmmo BulletAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FAmmo ShotgunAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FAmmo PlasmaAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FAmmo ExplosiveAmmo;

private:
	bool bIsSprinting;
	bool bFireIsPressed;

	float CurrentHealth;
	float MaxHealth;
	float CurrentArmor;
	float MaxArmor;

	FAmmo* CurrentAmmo;
	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;
	TArray<float> HealthPacks;
};
