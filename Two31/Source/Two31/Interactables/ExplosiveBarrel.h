#pragma once

#include "Interactables/Interactable.h"
#include "ExplosiveBarrel.generated.h"


UCLASS()
class TWO31_API AExplosiveBarrel : public AInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BarrelMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ExplosiveRadius;

public:
	AExplosiveBarrel();

	void BeginPlay();
	void Tick(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ImpulsePowah;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ExposionParticleScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ExposiveDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float DamageThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* ExplosionParticle;

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION()
	void OnExplosionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bExplode;
	bool bOldExplode;

	float DefaultExplosiveRadius;
};
