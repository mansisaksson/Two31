#pragma once
#include "Characters/EnemyCharacter.h"
#include "../Utilities/WeaponGlobals.h"
#include "CultistCharacter.generated.h"

struct SpottedPositions 
{
	bool bCanSeePlayerChest = false;
	bool bCanSeePlayerShoulder_Left = false;
	bool bCanSeePlayerShoulder_Right = false;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* Tiden det tar efter att fienden tappat siken om spelaren innan den går idle */
	float TimeToIdle;

private:
	void CheckLineOfSight();
	void FireTowardsPlayer();
	void FocusOnPosition(FVector Position);
	void ReactToPlayerMovement(float DeltaTime);
	void AvoidPlayer();

	void TryGetLineOfSight();
	void GoCloseToLastKnowPosition();
	void GuardLastKnownPosition();

	bool bHasLineOfSight;
	float TimeSinceLostLineOfSight;

	TArray<SpottedPositions> SpottedPlayerPositions;

	int AmmoPool;
	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;
};
