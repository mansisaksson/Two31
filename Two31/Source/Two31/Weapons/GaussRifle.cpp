#include "Two31.h"
#include "GaussRifle.h"
#include "../StatsPornManager.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"

AGaussRifle::AGaussRifle()
	: AWeapon()
{
	ClipSize = 30;
	RPM = 100;
	FullReloadTime = 2.f;
	FastReloadTime = 1.5f;
	timeSinceFire = 0.f;

	HeatParam = 0.f;

	HeatDissipationScale = 0.2f;
	HeatAccumulationScale = 0.3f;
	MaxHeatAccumulation = 10.f;

	MuzzleSpawnLocation = CreateDefaultSubobject<USceneComponent>("MuzzleSpawnLocation");
	MuzzleSpawnLocation->AttachTo(WeaponMesh, TEXT("BulletSpawn"));
}

void AGaussRifle::BeginPlay()
{
	AWeapon::BeginPlay();

	MatInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, WeaponMesh->GetMaterial(0));
	WeaponMesh->SetMaterial(0, MatInstance);
}

void AGaussRifle::Tick(float DeltaTime)
{
	AWeapon::Tick(DeltaTime);

	HeatParam = FMath::Clamp(HeatParam - DeltaTime * HeatDissipationScale, 0.f, 10.f);
	MatInstance->SetScalarParameterValue(TEXT("HeatParam"), HeatParam);
}

void AGaussRifle::FireShot(FVector TowardsLocation)
{
	if (AmmoPool == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ammo Pool assigned!"));
		return;
	}

	if ((*AmmoPool) > 0 && AmmoInClip > 0)
	{
		AWeapon::FireShot(TowardsLocation);

		AmmoInClip--;
		(*AmmoPool)--;

		HeatParam += HeatAccumulationScale;

		FHitResult result;
		ECollisionChannel collisionChannel;
		//collisionChannel = ECC_WorldDynamic;
		collisionChannel = ECC_GameTraceChannel2;
		FCollisionQueryParams collisionQuery;
		collisionQuery.bTraceComplex = true;
		collisionQuery.bReturnPhysicalMaterial = true;
		FCollisionObjectQueryParams objectCollisionQuery;
		objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
		FCollisionResponseParams collisionResponse;
		collisionResponse = ECR_Block;
		collisionQuery.AddIgnoredActor(this);
		collisionQuery.AddIgnoredActor(GetOwner());

		bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), TowardsLocation, collisionChannel, collisionQuery, collisionResponse);

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(MuzzeFlash, MuzzleSpawnLocation);
			//UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, BulletSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			particleComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		if (BulletTrail != NULL)
		{
			FVector BeamEndPoint = TowardsLocation;
			if (result.GetActor() != NULL)
				BeamEndPoint = result.Location;

			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrail, MuzzleSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			particleComp->SetBeamTargetPoint(0, BeamEndPoint, 0);
			particleComp->SetBeamTargetPoint(1, BeamEndPoint, 0);
		}

		if (hitObject)
			OnWeaponHit(result);
	}
}




