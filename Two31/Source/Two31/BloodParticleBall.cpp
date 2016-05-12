
#include "Two31.h"
#include "BloodParticleBall.h"

ABloodParticleBall::ABloodParticleBall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.1f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABloodParticleBall::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	Speed = 1000.0f;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = 9999999999999.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	LifetimeDestroy = 3.0f;

}

void ABloodParticleBall::BeginPlay()
{
	Super::BeginPlay();
}

void ABloodParticleBall::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	LifeTime += DeltaTime;
	if (LifeTime > LifetimeDestroy)
	{
		Destroy();
	}
}

void ABloodParticleBall::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{

	float speedPercentage = LifeTime / LifetimeDestroy;

	float MaxDecalSize = 1000*speedPercentage;
	float MinDecalSize = MaxDecalSize;

	if (Decal != NULL)
	{
		float DecalScale = (MaxDecalSize - MinDecalSize) * FMath::FRand() + MinDecalSize;
		FVector DecalSize = FVector(DecalScale, DecalScale, 1.f);
		FVector Normal = (HitResult.Normal * -1);
		UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(Decal, DecalSize, HitResult.GetComponent(), HitResult.BoneName, HitResult.Location, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition);
		DecalComp->SetWorldRotation(Normal.Rotation());
		DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
	}
	Destroy();
}