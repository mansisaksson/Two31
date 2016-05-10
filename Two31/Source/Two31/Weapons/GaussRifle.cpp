#include "Two31.h"
#include "GaussRifle.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

AGaussRifle::AGaussRifle()
	: AWeapon()
{
	ClipSize = 30;
	RPM = 100;
	FullReloadTime = 2.f;
	FastReloadTime = 1.5f;
	timeSinceFire = 0;
}

void AGaussRifle::FireShot(FVector TowardsLocation)
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
			//UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, BulletSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
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
		{
			OnWeaponHit(result);
			if (MuzzeFlash != NULL)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, result.Location, FRotator::ZeroRotator, true);

			if (result.GetActor() != NULL)
			{
				if (Cast<AEnemyCharacter>(result.GetActor()))
				{
					if (result.BoneName.GetPlainNameString() == "Head")
						Debug::LogOnScreen(result.BoneName.GetPlainNameString());
					AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(result.GetActor());
					TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					FDamageEvent DamageEvent(ValidDamageTypeClass);
					if (result.BoneName.GetPlainNameString() == "Head")
						Enemy->TakeDamage(WeaponDamage * HeadshotMultiplier, DamageEvent, result.GetActor()->GetInstigatorController(), this);
					else
						Enemy->TakeDamage(WeaponDamage, DamageEvent, result.GetActor()->GetInstigatorController(), this);

					FVector HitAngle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
					HitAngle.Normalize();
					Enemy->AddDelayedImpulse(HitAngle * ImpulsePowah, result.Location);
				}
				else if (Cast<APlayerCharacter>(result.GetActor()))
				{
					APlayerCharacter* Player = Cast<APlayerCharacter>(result.GetActor());
					TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					FDamageEvent DamageEvent(ValidDamageTypeClass);
					Player->TakeDamage(WeaponDamage, DamageEvent, result.GetActor()->GetInstigatorController(), this);
				}
				else if (result.GetComponent() != NULL && result.GetComponent()->Mobility == EComponentMobility::Movable && result.GetComponent()->IsSimulatingPhysics())
				{
					FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
					Angle.Normalize();
					result.GetComponent()->AddImpulseAtLocation(Angle * ImpulsePowah, result.Location);
				}
			}
		}
	}
}



