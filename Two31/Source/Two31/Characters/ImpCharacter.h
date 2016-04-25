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

	virtual void NotifyActorBeginOverlap(AActor* actor) override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = SetFunction)
		void SetTimeToMoveUpdate(float Time);

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

	virtual void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float RangeToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float RotationTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float TimeToIdle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float TimeToMoveUpdate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float ExtraTimeToMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float ExtraTimeToGetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float DistOffsetInSearch;

private:
	float TimeSinceRotationStart;
	float TimeSinceLastAttack;
	float TimeSinceLostLineOfSight;
	float TimeSinceMoveUpdate;

	bool bAttackOnCooldown;

	float DistanceToPlayer;

	FVector LastKnowPosition;
	bool bRandomSearch;
	bool bPlayerHasBeenSpotted;
	bool bOldLineOfSight;
	bool bLineOfSight;
	bool bMoveToLastKnown;
};
