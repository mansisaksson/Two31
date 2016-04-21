#include "Two31.h"
#include "SMG.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

ASMG::ASMG()
	: AWeapon()
{
	ClipSize = 30;
	RPM = 500;
	ReloadTime = 2.f;
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

		FHitResult result;
		ECollisionChannel collisionChannel;
		collisionChannel = ECC_WorldDynamic;
		FCollisionQueryParams collisionQuery;
		collisionQuery.bTraceComplex = true;
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
			//AWeapon::OnWeaponHit_Implementation(result);
			//AWeapon::OnWeaponHit(result);
			//OnWeaponHit_Implementation(result);
			OnWeaponHit(result);
			if (MuzzeFlash != NULL)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, result.Location, FRotator::ZeroRotator, true);

			if (result.GetComponent() != NULL && result.GetComponent()->Mobility == EComponentMobility::Movable && result.GetComponent()->IsSimulatingPhysics())
			{
				FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
				Angle.Normalize();
				result.GetComponent()->AddImpulseAtLocation(Angle * 50000.0f, result.Location);
			}

			if (result.GetActor() != NULL)
			{
				if (Cast<AEnemyCharacter>(result.GetActor()))
				{
					AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(result.GetActor());
					TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					FDamageEvent DamageEvent(ValidDamageTypeClass);
					Enemy->TakeDamage(WeaponDamage, DamageEvent, result.GetActor()->GetInstigatorController(), this);
				}
				else if (Cast<APlayerCharacter>(result.GetActor()))
				{
					APlayerCharacter* Player = Cast<APlayerCharacter>(result.GetActor());
					TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					FDamageEvent DamageEvent(ValidDamageTypeClass);
					Player->TakeDamage(WeaponDamage, DamageEvent, result.GetActor()->GetInstigatorController(), this);
				}
			}
		}
	}
}

/*
void ASMG::OnWeaponHit(FHitResult HitResult)
{

	OnWeaponHit2(HitResult);

	if (HitResult.GetComponent() != NULL)
	{
		UDestructibleComponent* DstrComp = Cast<UDestructibleComponent>(HitResult.GetComponent());
		if (DstrComp)
		{
			DstrComp->ApplyRadiusDamage(1, HitResult.Location, 1, 5000, false);
		}
	}

}
*/