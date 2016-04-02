#include "Two31.h"
#include "RocketLauncher.h"
#include "Projectile.h"

ARocketLauncher::ARocketLauncher()
	: AWeapon()
{

}

void ARocketLauncher::BeginPlay()
{
	AWeapon::BeginPlay();

}

void ARocketLauncher::Tick(float DeltaTime)
{
	AWeapon::Tick(DeltaTime);

}

void ARocketLauncher::StartFire(FVector TowardsLocation)
{
	AWeapon::StartFire(TowardsLocation);

	if (ProjectileClass != NULL)
	{
		FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
		const FRotator SpawnRotation = Angle.Rotation();
		const FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(MuzzeFlash, BulletSpawnLocation);
			FTransform particleTransform = particleComp->GetRelativeTransform();
			particleTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));
			particleComp->SetRelativeTransform(particleTransform);
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

void ARocketLauncher::UpdateFire(float DeltaSeconds, FVector TowardsLocation)
{
	AWeapon::UpdateFire(DeltaSeconds, TowardsLocation);
}

void ARocketLauncher::StopFire(FVector TowardsLocation)
{
	AWeapon::StopFire(TowardsLocation);
}


