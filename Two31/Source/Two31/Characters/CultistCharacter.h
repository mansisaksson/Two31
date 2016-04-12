#pragma once
#include "Characters/EnemyCharacter.h"
#include "../Utilities/WeaponGlobals.h"
#include "CultistCharacter.generated.h"

UCLASS()
class TWO31_API ACultistCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ACultistCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);

	bool EquipWeapon(TSubclassOf<class AWeapon> Weapon);
	void SelectWeaponSlot(int index);
	int GetWeaponIndex();
	void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<class AWeapon> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TurnRate;

private:
	void CheckLineOfSight();
	void FireTowardsPlayer();
	void FocusOnPlayer(float DeltaTime);
	void ReactToPlayerMovement(float DeltaTime);
	
	bool bHasLineOfSight;
	bool bCanSeePlayerChest;
	bool bCanSeePlayerShoulder_Left;
	bool bCanSeePlayerShoulder_Right;

	float TimeSinceSeenPlayer;
	float TimeToLooseLineOfSight;

	int AmmoPool;
	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;

};
