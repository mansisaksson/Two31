#pragma once

#include "GameFramework/Actor.h"
#include "Confetti.generated.h"

UCLASS()
class TWO31_API AConfetti : public AActor
{
	GENERATED_BODY()
	
public:	
	AConfetti();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	
	
};
