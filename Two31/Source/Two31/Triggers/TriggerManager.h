#pragma once
#include "GameFramework/Actor.h"
#include "TriggerManager.generated.h"

UCLASS()
class TWO31_API ATriggerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATriggerManager();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerBox;
};
