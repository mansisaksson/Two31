#pragma once
#include "Characters/EnemyCharacter.h"
#include "../Weapons/WeaponGlobals.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	TSubclassOf<class AWeapon> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* The Movespeed of the enemy when it's triggered */
	float TriggeredMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* Tiden det tar efter att fienden tappat siken om spelaren innan den går idle */
	float TimeToIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* How fast the enemy will fire it's burst */
	float RPM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* The minimum amount of bullets the enemy will shoot before pausing */
	int32 MinBurstSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* The maximum amount of bullets the enemy will shoot before pausing */
	int32 MaxBurstSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* The minimum amount if time the enemy will pause before bursting again */
	float MinBurstPause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_General)
	/* The maximum amount if time the enemy will pause before bursting again */
	float MaxBurstPause;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* Will be more prone to move when taking damage (0 - 100) 100 = 100% chance to move */
	float AvoidDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	float TimeToRandMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* A Random amount if time (in seconds) that will be added to the TimeToRandMove (Helps if enemies are spawned in a group) */
	float AddedRandomTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* After this amount of time the RandMove might get overridden by other movement*/
	float TimeGivenToMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* After the AI has lost track of the player it will move towards the last known position of the player (this is the amount of time it is given to do so) */
	float TimeToMoveTooLastKnownPosititon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* If true, TimeToRandMove will be ignored if a better line of sight is needed */
	bool bPrioritizeLineOfSight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* After loosing line of sight the enemy will try to re-gain it for this amount if time (includes side-stepping and relocating) */
	float TimeToGetLineOfSight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	bool bInstaReactToCornerPeaking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	float MinRandMoveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	float MaxRandMoveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* When the player gets withing this radius the enemy will start to back off */
	float RetreatDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* The distance of which the enemy will back off */
	float RetreatRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* If an obstacle is within this radius the enemy might not move in that direction */
	float RetreatNavRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Triggerd)
	/* When the player gets outside this radius the enemy will start to run towards the player */
	float HuntDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Search)
	/* When the player gets outside this radius the enemy will start to run towards the player */
	float SearchRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Search)
	/* When the player gets outside this radius the enemy will start to run towards the player */
	float SearchMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_Search)
	/* The amount of time the enemy will spend searching before going idle */
	float SearchTime;

private:
	void CheckLineOfSight();
	void FireTowardsPlayer(float DeltaTime);
	void FocusOnPosition(float DeltaTime, FVector Position);
	void ReactToPlayerMovement(float DeltaTime);
	void AvoidPlayer(float DeltaTime);

	void TryGetLineOfSight(float DeltaTime);
	void GoToLastKnownPosition(float DeltaTime);
	void SearchForPlayer(float DeltaTime);

	int AmmoPool;
	int TimesShot;
	int BurstSize;
	int LastFloorValue;

	bool bHasLineOfSight;
	bool bOldHasLineOfSight;
	bool bIsRandMoving;
	bool bLostLineOfSight;
	bool bRandMoveAwayFromPlayer;
	bool bHasMovedToPlayer;

	float DefaultTimeToRandMove;
	float TimeSinceLostLineOfSight;
	float TimeSinceRandMovement;
	float TimeSinceRanToLastKnownPosition;
	float TimeSinceBurstPause;
	float TimeToBurstPause;
	float TimeSinceShotFired;
	float TimeSinceStartSearch;

	FVector LastKnownPlayerPos;
	FVector LastKnownPlayerVelocity;
	FRotator OldRotation;

	class AWeapon* CurrentWeapon;
	TArray<class AWeapon*> WeaponSlots;
	TArray<SpottedPositions> SpottedPlayerPositions;
	SpottedPositions LastKnownSpottedPositions;
};
