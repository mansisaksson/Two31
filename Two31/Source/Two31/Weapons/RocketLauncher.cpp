#include "Two31.h"
#include "RocketLauncher.h"
#include "Projectile.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

ARocketLauncher::ARocketLauncher()
	: AWeapon()
{
	ClipSize = 8;
}

void ARocketLauncher::FireShot(FVector TowardsLocation)
{
	AWeapon::FireShot(TowardsLocation);

	if (AmmoPool == NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ammo Pool assigned!"));
		return;
	}

	if ((*AmmoPool) > 0 && AmmoInClip > 0)
	{
		AmmoInClip--;
		(*AmmoPool)--;

		if (ProjectileClass != NULL)
		{
			FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
			const FRotator SpawnRotation = Angle.Rotation();
			const FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			Projectile->GetCollisionComp()->IgnoreActorWhenMoving(this, true);
			Projectile->GetCollisionComp()->IgnoreActorWhenMoving(GetOwner(), true);

			if (MuzzeFlash != NULL)
			{
				UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(MuzzeFlash, BulletSpawnLocation);
				FTransform particleTransform = particleComp->GetRelativeTransform();
				particleTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));
				particleComp->SetRelativeTransform(particleTransform);
			}

			if (ArmFireAnimation != NULL && OwnerMesh != NULL)
			{
				if (Cast<APlayerCharacter>(OwnerMesh->GetAttachmentRootActor()))
				{
					UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
					if (AnimInstance != NULL)
						AnimInstance->Montage_Play(ArmFireAnimation, 1.f);
				}
				else if (Cast<ACultistCharacter>(OwnerMesh->GetAttachmentRootActor()))
				{
					// Play Animation
				}
			}
		}
	}
}