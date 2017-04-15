// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BloodParticleBall.generated.h"

UCLASS()
class TWO31_API ABloodParticleBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABloodParticleBall();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UMaterialInterface* Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float LifetimeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float Speed;

	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:

	float LifeTime = 0.0f;

};
