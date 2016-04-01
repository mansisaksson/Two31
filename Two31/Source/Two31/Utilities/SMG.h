#pragma once

#include "Weapon.h"
#include "SMG.generated.h"

UCLASS()
class TWO31_API ASMG : public AWeapon
{
	GENERATED_BODY()
	
public:
	ASMG();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartFire(FVector TowardsLocation) override;
	virtual void UpdateFire(FVector TowardsLocation) override;
	virtual void EndFire(FVector TowardsLocation) override;

};
