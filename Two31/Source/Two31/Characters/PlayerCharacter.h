#pragma once

#include "GameFramework/Character.h"
#include "../Weapons/WeaponGlobals.h"
#include "PlayerCharacter.generated.h"

struct SInventory
{
	int32 ID = 0;
	FString Name = "None";
};

UCLASS()
class TWO31_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPArmMesh;

	/** First person camera */
	UPROPERTY(VisibleDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCamera;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnNoiseEmitterComponent* NoiseEmitter;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* MeleeCollider;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LineOfSight_Chest;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LineOfSight_Shoulder_Right;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LineOfSight_Shoulder_Left;

public:
	APlayerCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USkeletalMeshComponent* GetFPMeshArmMesh() const { return FPArmMesh; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FPCamera; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USceneComponent* GetPlayerHitPoint_Chest() { return LineOfSight_Chest; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USceneComponent* GetPlayerHitPoint_Shoulder_Right() { return LineOfSight_Shoulder_Right; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USceneComponent* GetPlayerHitPoint_Shoulder_Left() { return LineOfSight_Shoulder_Left; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetHealth() { return CurrentHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetHealthPacks() { return HealthPacks.Num(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetArmor() { return CurrentArmor; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxArmor() { return MaxArmor; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetTimeSinceMelee() { return TimeSinceMelee; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetDefaultMeleeRadius() { return DefaultMeleeRadius; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmountOfWeapons();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	class AWeapon* GetCurrentWeapon() { return CurrentWeapon; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	class AWeapon* GetNextWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool GetADS() { return bADS; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmmoPool() { if (CurrentAmmo != NULL) { return CurrentAmmo->AmmoPool; } return 0; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetMaxAmmo() { if (CurrentAmmo != NULL) { return CurrentAmmo->MaxAmmo; } return 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetAmountOfItems();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	FString GetItemName(class AItemPickup* ItemToName);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetItemID(class AItemPickup* ItemToName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetItem(int32 itemID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	bool PlayerHasItem(int32 ItemID);

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, Category = Event)
	void PickedUpItem(AActor* OtherActor);
	void PickedUpItem_Implementation(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetIndicatorLocation() { return IndicatorLocation; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetIndicatorOpacity() { return IndicatorOpacity; }

	UFUNCTION(BlueprintCallable, Category = SetFunction)
	void SetMeleeRadius(float Radius);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UFUNCTION()
	void OnMeleeBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	bool EquipWeapon(TSubclassOf<class AWeapon> Weapon);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void OnFire();
	void OnReleaseFire();
	void OnReload();
	void OnADS();
	void OnMeleeAttack();

	bool PickupHealthPack(class AHealthPickup* Healthpack);
	bool ChangeArmor(float pChange);
	bool AddAmmo(EAmmoType Ammo, int Amount);
	bool AddItem(AItemPickup* item);

	void SelectWeaponSlot(int index);
	void SelectWeaponSlot1();
	void SelectWeaponSlot2();
	void SelectWeaponSlot3();
	void SelectWeaponSlot4();
	void NextWeapon();
	void PreviousWeapon();
	int GetWeaponIndex();

	void MoveForward(float Val);
	void MoveSideways(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void UseHealthPack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float StartingArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ArmorAbsorption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ViewPitchMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ViewPitchMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float DefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ADSFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MeleeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MeleePowah;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MeleeTime;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float IndicatorDisplayTime;

	float GetDamageCauserLocation(AActor* DamageCauser);
private:
	bool bFireIsPressed;
	bool bADS;
	bool bMeleeAttack;

	float CurrentHealth;
	float CurrentArmor;
	float MaxArmor;
	float LastFootstep;
	float TimeSinceMelee;
	float DefaultMeleeRadius;

	float IndicatorLocation;
	float IndicatorTimer;
	float IndicatorOpacity;

	FVector DefaultArmLocation;
	FRotator DefaultArmRotation;

	FAmmo* CurrentAmmo;
	class AWeapon* CurrentWeapon;
	class APlayerController* PlayerController;

	TArray<class AWeapon*> WeaponSlots;
	TArray<float> HealthPacks;

	TArray<int32> Items;
	TArray<SInventory> Inventory;
	TArray<AActor*> MeleedActors;
};
