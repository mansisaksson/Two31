#include "Two31.h"
#include "ExplosiveBarrel.h"
#include "../Characters/EnemyCharacter.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	ImpulsePowah = 90000;
	ExposionParticleScale = 5.f;
	ExposiveDamage = 100.f;
	DamageThreshold = 30.f;
	bExplode = false;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	SetRootComponent(BarrelMesh);

	ExplosiveRadius = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosiveRadius"));
	ExplosiveRadius->SetSphereRadius(100.f);
	ExplosiveRadius->AttachTo(BarrelMesh);
	ExplosiveRadius->SetCollisionResponseToAllChannels(ECR_Overlap);
	ExplosiveRadius->bGenerateOverlapEvents = true;
	ExplosiveRadius->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnExplosionBeginOverlap);
}

void AExplosiveBarrel::BeginPlay()
{
	DefaultExplosiveRadius = ExplosiveRadius->GetUnscaledSphereRadius();
	ExplosiveRadius->SetSphereRadius(0.f);
}

void AExplosiveBarrel::Tick(float DeltaTime)
{
	if (bExplode && !bOldExplode)
	{
		if (ExplosionParticle != NULL)
		{
			UParticleSystemComponent* ExplosionComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
			float ExplosionScale = ((GetActorScale().X + GetActorScale().Y + GetActorScale().Z) / 3.f) * ExposionParticleScale;
			ExplosionComponent->SetWorldScale3D(FVector(ExplosionScale, ExplosionScale, ExplosionScale));
		}
		ExplosiveRadius->SetSphereRadius(DefaultExplosiveRadius);
	}

	if (bExplode && bOldExplode)
	{
		Destroy();
	}

	bOldExplode = bExplode;
}

void AExplosiveBarrel::OnExplosionBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	FVector Distance = OtherActor->GetActorLocation() - GetActorLocation();
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	OtherActor->TakeDamage(ExposiveDamage * (1.0f - FMath::Clamp(Distance.Size() / DefaultExplosiveRadius, 0.0f, 1.0f)), DamageEvent, OtherActor->GetInstigatorController(), this);

	FVector Angle = OtherComp->GetComponentLocation() - GetActorLocation();
	Angle.Normalize();
	if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor))
		enemy->AddDelayedImpulse(Angle * ImpulsePowah, enemy->GetActorLocation());

	else if (OtherComp->Mobility == EComponentMobility::Movable && OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulse(Angle * ImpulsePowah);
}

float AExplosiveBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageThreshold = FMath::Clamp(DamageThreshold - DamageAmount, 0.f, 100.f);

	if (DamageThreshold <= 0)
		bExplode = true;

	return DamageAmount;
}