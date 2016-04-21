#pragma once
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Triggered		UMETA(DisplayName = "Triggered"),
	Search			UMETA(DisplayName = "Search"),
	Idle			UMETA(DisplayName = "Idle"),
	NumberOfStates	UMETA(DisplayName = "DoNotUse")
};

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

	virtual float GetHealth();

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

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
	class APlayerCharacter* PlayerReferense;

	float CurrentHealth;
	float TimeSinceDeath;

	bool bIsAlive;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	EEnemyState GetCurrentState() { return EnemyState; }
	UFUNCTION(BlueprintCallable, Category = SetFunction)
	void SetCurrentState(EEnemyState State);

private:
	UPROPERTY(EditAnywhere, Category = Gameplay)
	EEnemyState EnemyState;

};

