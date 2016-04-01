#include "Two31.h"
#include "SMG.h"

ASMG::ASMG()
	: AWeapon()
{

}

void ASMG::BeginPlay()
{
	AWeapon::BeginPlay();

	//UGameplayStatics::SpawnEmitterAttached(SprayParticles.ElectricitySpray, CurrentStaff->GetStaticMesh());
}

void ASMG::Tick(float DeltaTime)
{
	AWeapon::Tick(DeltaTime);

}

void ASMG::StartFire(FVector TowardsLocation)
{

}

void ASMG::UpdateFire(FVector TowardsLocation)
{
	if (MuzzeFlash != NULL)
	{
		UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, BulletSpawnLocation->GetCustomLocation(), FRotator::ZeroRotator, true);
		FTransform particleTransform = particleComp->GetRelativeTransform();
		particleTransform.SetScale3D(FVector(0.2f, 0.2f, 0.2f));
		particleComp->SetRelativeTransform(particleTransform);
	}

	FHitResult result;
	ECollisionChannel collisionChannel;
	collisionChannel = ECC_WorldDynamic;
	FCollisionQueryParams collisionQuery;
	collisionQuery = FCollisionQueryParams::DefaultQueryParam;
	FCollisionObjectQueryParams objectCollisionQuery;
	objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	FCollisionResponseParams collisionResponse;
	collisionResponse = ECR_Overlap;
	collisionQuery.AddIgnoredActor(this);

	bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetCustomLocation(), TowardsLocation, collisionChannel, collisionQuery, collisionResponse);

	if (hitObject)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, result.Location, FRotator::ZeroRotator, true);
	}
}

void ASMG::EndFire(FVector TowardsLocation)
{

}



