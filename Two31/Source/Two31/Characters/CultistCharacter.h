#pragma once

#include "Characters/EnemyCharacter.h"
#include "CultistCharacter.generated.h"


UCLASS()
class TWO31_API ACultistCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ACultistCharacter();
	
protected:
	UFUNCTION()
	virtual void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
	virtual void OnSeePawn(APawn *OtherPawn);

private:

	float Shoot;
	
	float StepRight;
	float StepLeft;
	float BackUp;
	float MoveForward;

	float LookForPlayer;
};
