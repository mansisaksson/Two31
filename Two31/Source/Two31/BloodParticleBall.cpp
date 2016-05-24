
#include "Two31.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "BloodParticleBall.h"

ABloodParticleBall::ABloodParticleBall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.1f);
	//CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCanEverAffectNavigation(false);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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

void ABloodParticleBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionComp->OnComponentHit.AddDynamic(this, &ABloodParticleBall::OnHit);
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
	if ((OtherActor != NULL))
	{
		if (Cast<APlayerCharacter>(OtherActor) || Cast<AEnemyCharacter>(OtherActor) || OtherActor->IsA(ABloodParticleBall::StaticClass()))
		{
			CollisionComp->IgnoreActorWhenMoving(OtherActor, true);
		}
		else
		{
			float speedPercentage = 1.0f - (LifeTime / LifetimeDestroy);

			float MaxDecalSize = 100 * speedPercentage;
			float MinDecalSize = 50 * speedPercentage;

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
	}
}