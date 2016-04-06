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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	UFUNCTION()
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	UNavigationSystem* NavSystem;
	class AAIController* AIController;

	bool bIsChasingPlayer;

	float timeSinceSeenPlayer;
};

