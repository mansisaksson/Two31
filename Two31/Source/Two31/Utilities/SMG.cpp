#include "Two31.h"
#include "SMG.h"
#include "Engine.h"

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

		bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), TowardsLocation, collisionChannel, collisionQuery, collisionResponse);

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, BulletSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			FTransform particleTransform = particleComp->GetRelativeTransform();
			particleTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));
			particleComp->SetRelativeTransform(particleTransform);
		}

		if (hitObject)
		{
			if (MuzzeFlash != NULL)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, result.Location, FRotator::ZeroRotator, true);

			if (result.GetComponent() != NULL && result.GetComponent()->Mobility == EComponentMobility::Movable)
			{
				FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
				Angle.Normalize();
				result.GetComponent()->AddImpulseAtLocation(Angle * 50000.0f, result.Location);
			}
		}

		if (FireSound != NULL)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

		if (ArmFireAnimation != NULL && ArmMesh != NULL)
		{
			UAnimInstance* AnimInstance = ArmMesh->GetAnimInstance();
			if (AnimInstance != NULL)
				AnimInstance->Montage_Play(ArmFireAnimation, 1.f);
		}
	}
}

