#pragma once

#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class TWO31_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
