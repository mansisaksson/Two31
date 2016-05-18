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
	class USphereComponent* L_ClawRadius;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* R_ClawRadius;
	
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
	UFUNCTION(BlueprintCallable, Category = GetFunction)
	bool GetPlayAttackAnimation() { return bPlayAttackAnimation; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	float GetDefaultClawRadius() { return DefaultClawRadius; }

	UFUNCTION(BlueprintCallable, Category = SetFunction)
	void SetClawRadius(float LeftRadius, float RightRadius);

protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);
	UFUNCTION()
	virtual void GetOverlappingActors(UShapeComponent* Sphere, UClass* ClassFilter);
	UFUNCTION()
	virtual void OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	bool CanSeePlayer();
	float GetDistanceToPlayer();
	bool AtLastKnownPosition();
	void GetPositionOfImps();

	void RotateTowardsPlayer();
	void MoveToPlayersEstimatedPosition();

	bool PathFidningQuery(FVector Position);

	bool WallInWay(FVector Position);

	void SetRunAroundDegree(float Min, float Max);
	void MoveAroundPlayer();
	void Reposition();
	void FocusOnPosition(FVector Location);
	void Attack();

	virtual void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackMoveCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MinAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float LandCooldown;
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

	// Determines wehther the imp will attempt to run around the player ( if conditions allow for it ) or not. true == run around if possible.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	bool bMoveAroundPlayer;

	// Minimum amount of degrees the imp can attempt to run if flanking, only applies to imps on the left and right sides. Default at 50.f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float MinFlankDegree;
	// Maximum amount of degrees the imp can attempt to run if flanking, only applies to imps on the left and right sides. Default at 90.f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float MaxFlankDegree;

	// 0 - MaxDegreeLocatedRight are the imps to the right of the current one. Standard is 70, lowering the value will create a smaller amount of imps flanking right.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float MaxDegreeLocatedRight;
	// MinDegreeLocatedLeft - 180 are the imps to the left of the current one. Standard is 110, increasing the value will create a smaller amount of imps flanking left.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float MinDegreeLocatedLeft;

	// Distance the imp allowed from the player, the distance is the distance when triggered + this offset. If the imp is further away it will change move to location and run towards the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behaviour)
	float AllowedDistanceFromPlayerOffset;

private:
	float TimeSinceLand;
	float TimeSinceRotationStart;
	float TimeSinceLastAttack;
	float TimeSinceLostLineOfSight;
	float TimeSinceMoveUpdate;
	float ExtraTimeToMove;
	float DefaultClawRadius;

	float MoveAroundTimer;

	bool bAttackOnCooldown;

	FVector LastKnowPosition;
	bool bMoveCloser;
	bool bMoveOnce;
	bool bRandomSearch;
	bool bPlayerHasBeenSpotted;
	bool bOldLineOfSight;
	bool bLineOfSight;
	bool bMoveToLastKnown;
	bool bRepositioned;
	bool bPlayAttackAnimation;

	bool bForceMovement;
	FVector ForcedMovementDirection;
	FVector MoveAroundLocation;

	float MaxWalkSpeed;
	float HalfWalkSpeed;

	float MoveAroundLocationMin;
	float MoveAroundLocationMax;

	float AllowedDistanceFromPlayer;

	FVector PlayerPositionedWhenAggro;
	FVector PlayerForwardVectorWhenAggro;

	FRotator OldRotation;
};
