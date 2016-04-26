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

struct FImpulseValues
{
	FVector Impulse;
	FVector Location;
};

UCLASS(config=Game)
class AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Sensor, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensor;

protected:
	UPROPERTY(VisibleAnywhere, Category = Trigger)
	class USphereComponent* AlertRadius;

public:
	AEnemyCharacter();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = SetFunction)
	virtual void SetCurrentState(EEnemyState State);
	virtual void AddDelayedImpulse(FVector Impulse, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	EEnemyState GetCurrentState() { return EnemyState; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	virtual float GetHealth() { return CurrentHealth; }
protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);
	UFUNCTION()
	void GetOverlappingActors(UShapeComponent* Sphere, UClass* ClassFilter);

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

private:
	UPROPERTY(EditAnywhere, Category = Gameplay)
	EEnemyState EnemyState;

	TArray<FImpulseValues> DelayedImpulses;
};

