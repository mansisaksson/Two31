#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TWO31_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPArmMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPGunMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* GunLocation;

public:
	APlayerCharacter();

	FORCEINLINE USkeletalMeshComponent* GetFPMeshArmMesh() const { return FPArmMesh; }
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetArmor();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetMaxArmor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int GetClipSize();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int GetReserveAmmo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	int GetMaxAmmo();

protected:
	void OnFire();

	void MoveForward(float Val);
	void MoveSideways(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void StartSprint();
	void StopSprint();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

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
	TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

private:
	bool bIsSprinting;

	float CurrentHealth;
	float MaxHealth;
	float CurrentArmor;
	float MaxArmor;

	int ClipSize;
	int CurrentAmmo;
	int ReserveAmmo;
	int MaxAmmo;
};
