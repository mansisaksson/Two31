#pragma once
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS(config=Game)
class AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Sensor, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensor;

public:
	AEnemyCharacter();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	virtual void Take_Damage(float Damage);

	virtual float GetHealth();

protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);

	virtual void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float DespawnTimer;

	UNavigationSystem* NavSystem;
	class AAIController* AIController;

	float CurrentHealth;
	float TimeSinceDeath;

	bool bIsAlive;
};

