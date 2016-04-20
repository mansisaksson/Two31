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
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TSubclassOf<class AWeapon> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* Tiden det tar efter att fienden tappat siken om spelaren innan den går idle */
	float TimeToIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* Will be more prone to move when taking damage (0 - 100) 100 = 100% chance to move */
	float AvoidDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float TimeToRandMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* A Random amount if time (in seconds) that will be added to the TimeToRandMove (Helps if enemies are spawned in a group) */
	float AddedRandomTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* After this amount of time the RandMove might get overridden by other movement*/
	float TimeGivenToMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* After the AI has lost track of the player it will move towards the last known position of the player (this is the amount of time it is given to do so) */
	float TimeToMoveTooLastKnownPosititon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	/* If true, TimeToRandMove will be ignored if a better line of sight is needed */
	bool bPrioritizeLineOfSight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	bool bInstaReactToCornerPeaking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float MinRandMoveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float MaxRandMoveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float RetreatDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float HuntDistance;

private:
	void CheckLineOfSight();
	void FireTowardsPlayer(float DeltaTime);
	void FocusOnPosition(float DeltaTime, FVector Position);
	void ReactToPlayerMovement(float DeltaTime);
	void AvoidPlayer(float DeltaTime);

	void TryGetLineOfSight(float DeltaTime);
	void GuardLastKnownPosition(float DeltaTime);

	int AmmoPool;
	bool bHasLineOfSight;
	bool bOldHasLineOfSight;
	bool bIsRandMoving;
	bool bLostLineOfSight;
	bool bRandMoveAwayFromPlayer;

	float DefaultTimeToRandMove;
	float TimeSinceLostLineOfSight;
	float TimeSinceRandMovement;
	float TimeSinceRanToLastKnownPosition;

	FVector LastKnownPlayerPos;

	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;
	TArray<SpottedPositions> SpottedPlayerPositions;
	SpottedPositions LastKnownSpottedPositions;
};
