#pragma once

#include "Characters/EnemyCharacter.h"
#include "ImpCharacter.generated.h"

struct SeenPositions
{
	bool bCanSeePlayerChest = false;
	bool bCanSeePlayerShoulder_Left = false;
	bool bCanSeePlayerShoulder_Right = false;
};

UCLASS()
class TWO31_API AImpCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* AttackRadius;
	
public:
	AImpCharacter();
	
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = Movement)
	void ForceMovement(FVector Direction);
	UFUNCTION(BlueprintCallable, Category = Movement)
	void StopForcedMovement();

protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);
	UFUNCTION()
	virtual void OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	bool CanSeePlayer();
	UFUNCTION()
	float GetDistanceToPlayer();
	UFUNCTION()
	bool AtLastKnownPosition();

	void RotateTowardsPlayer();
	void MoveToPlayersEstimatedPosition();

	void Reposition();
	void FocusOnPosition();
	void Attack();

	virtual void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackDamage;

	// Time the imp takes to rotate when triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float RotationTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float TimeToIdle;
	// How often the Imps destination will be updated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float TimeToMoveUpdate;

	// Extra Time given for the imp to get to a location before officially losing sight of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float ExtraTimeToGetLocation;

	// Distance the imp will go to the side after having attacked. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float SideStepDistance;

	// Minimum distance when searching for the player during random search.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float MinDistanceRandomSearch;
	// Extra Distance added to distance to player when creating max range for random search location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float DistOffsetInSearch;

private:
	float TimeSinceRotationStart;
	float TimeSinceLastAttack;
	float TimeSinceLostLineOfSight;
	float TimeSinceMoveUpdate;
	float ExtraTimeToMove;

	bool bAttackOnCooldown;

	FVector LastKnowPosition;
	bool bRandomSearch;
	bool bPlayerHasBeenSpotted;
	bool bOldLineOfSight;
	bool bLineOfSight;
	bool bMoveToLastKnown;
	bool bRepositioned;

	bool bForceMovement;
	FVector ForcedMovementDirection;

	float MaxWalkSpeed;
	float HalfWalkSpeed;

	FRotator OldRotation;
};
