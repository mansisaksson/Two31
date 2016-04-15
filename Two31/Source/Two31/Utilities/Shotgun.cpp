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

		const FName TraceTag("Debug Trace");

		FHitResult result;
		ECollisionChannel collisionChannel;
		collisionChannel = ECC_WorldDynamic;
		FCollisionQueryParams collisionQuery;
		collisionQuery.TraceTag = TraceTag;
		GetWorld()->DebugDrawTraceTag = TraceTag;
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
		int NumberOfShots = 8;
		float RadiusMax = 10.f;
		for (int i = 0; i < NumberOfShots; i++)
		{
			// ha 8 kvadranter, +45 grader vid varje skott - ha en radius på hur långt ut skotten kan komma
			float Step = (360.f / (float)NumberOfShots);
			float AngleMin = i * Step;
			float AngleMax = (i + 1) * Step;

			float Angle = FMath::FRandRange(AngleMin, AngleMax);
			//float Radius = FMath::FRandRange(-RadiusMax, RadiusMax);

			float Radius = 5.f;

			float playerRotationZ = GetOwner()->GetActorRotation().Yaw;
			float angleRadiants = FMath::DegreesToRadians(Angle);

			//TowardsLocation.Normalize();

			FVector DirectionVector = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
			DirectionVector.Normalize();
			//DirectionVector *= 5000.f;
			//FVector RelativeDirectionVector = GetTransform().TransformVectorNoScale(DirectionVector);
			
			FRotator Rotation = FRotator(Radius * FMath::Sin(angleRadiants), Radius * FMath::Sin(angleRadiants),0);
			FRotationMatrix RotationMatrix(Rotation);
			FTransform testTransform = FTransform();
			testTransform.SetToRelativeTransform(GetOwner()->GetTransform());
			
			//FVector RotatedDirVector = GetOwner()->GetTransform().TransformVector(DirectionVector);
			//FVector RotatedDirVector = RotationMatrix.TransformVector(DirectionVector);
			FVector RotatedDirVector = Rotation.RotateVector(DirectionVector);
			RotatedDirVector *= 5000.f;


			bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), BulletSpawnLocation->GetComponentLocation() + RotatedDirVector, collisionChannel, collisionQuery, collisionResponse);

			if (hitObject)
			{
				Debug::OnScreenMessage((result.GetActor()->GetName()));
				if (MuzzeFlash != NULL)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, result.Location, FRotator::ZeroRotator, true);

				if (result.GetComponent() != NULL && result.GetComponent()->Mobility == EComponentMobility::Movable && result.GetComponent()->IsSimulatingPhysics())
				{
					FVector HitAngle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
					HitAngle.Normalize();
					result.GetComponent()->AddImpulseAtLocation(HitAngle * 50000.0f, result.Location);
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