#pragma once
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS(config=Game)
class AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Sensor, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensor;

public:
	AEnemyCharacter();

	void PostInitializeComponents() override;

	void BeginPlay();
	void Tick(float DeltaTime);

	void InflictDamage(float Damage);

	float GetHealth();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UFUNCTION()
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);
	//UFUNCTION()
	//void OnComponentHit(APawn *OtherActor);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float MaxHealth;

private:
	UNavigationSystem* NavSystem;
	class AAIController* AIController;

	float CurrentHealth;

	bool bIsChasingPlayer;
	bool bIsAlive;
};

