#pragma once

#include "GameFramework/Character.h"
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
	bool ChangeHealth(float pChange);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetArmor();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxArmor();
	UFUNCTION(BlueprintCallable,Category = GetFunction)
	bool ChangeArmor(float pChange);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetClipSize();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetReserveAmmo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int32 GetMaxAmmo();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool EquipWeapon(TSubclassOf<class AWeapon> Weapon);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void OnFire();
	void OnReleaseFire();

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

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon1;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon2;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon3;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<class AWeapon> StarterWeapon4;

private:
	bool bIsSprinting;
	bool bFireIsPressed;

	float CurrentHealth;
	float MaxHealth;
	float CurrentArmor;
	float MaxArmor;

	int32 ClipSize;
	int32 CurrentAmmo;
	int32 ReserveAmmo;
	int32 MaxAmmo;

	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;
};
