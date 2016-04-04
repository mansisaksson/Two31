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
	virtual void UpdateFire(float DeltaSeconds, FVector TowardsLocation) override;
	virtual void StopFire(FVector TowardsLocation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimMontage* ArmFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UParticleSystem* MuzzeFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RPM;

private:
	void FireShot(FVector TowardsLocation);

	float timeSinceFire;
	float timeSinceReloadStart;
};
