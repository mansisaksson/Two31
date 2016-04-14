#include "Two31.h"
#include "Shotgun.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"


AShotgun::AShotgun()
	: AWeapon()
{
	ClipSize = 4;
	RPM = 60;
	ReloadTime = 1.f;
	timeSinceFire = 0;
}

void AShotgun::FireShot(FVector TowardsLocation)
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
		collisionQuery.AddIgnoredActor(GetOwner());

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, BulletSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			FTransform particleTransform = particleComp->GetRelativeTransform();
			particleTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));
			particleComp->SetRelativeTransform(particleTransform);
		}
		for (int i = 0; i < 3; i++)
		{
			//int32 random = FMath::Rand();
			float random = FMath::FRandRange(-1, 1);
			

			//FVector Angle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
			//Angle.Normalize();

			//FRotator rotation = TowardsLocation.Rotation();
			//float RandRadius = 0.1f;
			//float randomRot = FMath::FRandRange(-RandRadius, RandRadius);
			//float randomRot2 = FMath::FRandRange(-RandRadius, RandRadius);
			//float randomRot3 = FMath::FRandRange(-RandRadius, RandRadius);

			//FVector center = Angle * 5;
			//center.Y += randomRot;
			//center.Z += randomRot2;
			//center.X += randomRot3;
			//center.Normalize();
			//center *= 5000.f;
			////center.X += 10 + random;
			//FVector upper = Angle * 500.f;
			//upper.Z += 10;
			//FVector lower = Angle * 500.f;
			//lower.Z -= 10;


			/*FVector temp = FMath::VRandCone(TowardsLocation, 100.f);*/
			//temp = FVector(temp.X * random, temp.Y * random, temp.Z * random);

			//FVector temp = FMath::VRandCone(TowardsLocation, ConeHalf);
			//FRotator rotation = GetOwner()->GetActorRotation();

			//TowardsLocation = FVector( FMath::Sin(TowardsLocation.X),  FMath::Sin(TowardsLocation.Y ) , TowardsLocation.Z);
			/*TowardsLocation = FVector(TowardsLocation.X * FMath::Sin(30), TowardsLocation.Y, TowardsLocation.Z * FMath::Sin(30));*/
			// ha 8 kvadranter, +45 grader vid varje skott - ha en radius på hur långt ut skotten kan komma

			bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), TowardsLocation, collisionChannel, collisionQuery, collisionResponse);

			if (hitObject)
			{
				Debug::OnScreenMessage((result.GetActor()->GetName()));
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
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Damaging Enemy"));
						AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(result.GetActor());
						APlayerController* PlayerController = Cast<APlayerController>(result.GetActor()->GetInstigatorController());
						TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
						FDamageEvent DamageEvent(ValidDamageTypeClass);
						Enemy->TakeDamage(WeaponDamage, DamageEvent, PlayerController, this);
					}
				}
			}
		}

		if (FireSound != NULL)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

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