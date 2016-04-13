#pragma once

#include "Characters/EnemyCharacter.h"
#include "ImpCharacter.generated.h"

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

protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);
	UFUNCTION()
	virtual void OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemies)
	float RotationTimer;



	bool bIsChasingPlayer;
	bool bAttackOnCooldown;
	bool bAggro;

	float TimeSinceRotationStart;
	float TimeSinceLastAttack;

	FVector TargetLocation;
};
