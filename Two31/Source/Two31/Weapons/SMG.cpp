#include "Two31.h"
#include "SMG.h"
#include "../StatsPornManager.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

ASMG::ASMG()
	: AWeapon()
{
	ClipSize = 30;
	RPM = 500;
	FullReloadTime = 2.f;
	FastReloadTime = 1.5f;
	timeSinceFire = 0;
}

void ASMG::FireShot(FVector TowardsLocation)
{
	AWeapon::FireShot(TowardsLocation);

	if (AmmoPool == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ammo Pool assigned!"));
		return;
	}

	if ((*AmmoPool) > 0 && AmmoInClip > 0)
	{
		AmmoInClip--;
		(*AmmoPool)--;
		UStatsPornManager::IncreaseAmountOfShotsFired();

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
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(MuzzeFlash, BulletSpawnLocation);
			particleComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		if (BulletTrail != NULL)
		{
			FVector BeamEndPoint = TowardsLocation;
			if (result.GetActor() != NULL)
				BeamEndPoint = result.Location;

			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrail, BulletSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			particleComp->SetBeamTargetPoint(0, BeamEndPoint, 0);
		}

		if (hitObject)
			OnWeaponHit(result);
	}
}