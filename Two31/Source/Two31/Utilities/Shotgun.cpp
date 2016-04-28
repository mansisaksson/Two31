#include "Two31.h"
#include "Shotgun.h"
#include <iostream>
#include <sstream>
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"

AShotgun::AShotgun()
	: AWeapon()
{
	ClipSize = 4;
	RPM = 60;
	ReloadTime = 1.f;
	timeSinceFire = 0;
	RadiusMin = 0.f;
	RadiusMax = 10.f;
	Distance = 5000.f;
	NumberOfShots = 8;
	HeatParam = 0.f;
	ImpulsePowah = 900.f;

	HeatDissipationScale = 0.2f;
	HeatAccumulationScale = 0.3f;
	MaxHeatAccumulation = 10.f;

	MuzzleFlashLocation = CreateDefaultSubobject<USceneComponent>("MuzzleFlash");
	MuzzleFlashLocation->AttachTo(WeaponMesh, TEXT("MuzzleFlash"));
}

void AShotgun::BeginPlay()
{
	AWeapon::BeginPlay();
	
	MatInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, WeaponMesh->GetMaterial(0));
	WeaponMesh->SetMaterial(0, MatInstance);
}

void AShotgun::Tick(float DeltaTime)
{
	AWeapon::Tick(DeltaTime);
	HeatParam = FMath::Clamp(HeatParam - DeltaTime * HeatDissipationScale, 0.f, 10.f);
	MatInstance->SetScalarParameterValue(TEXT("HeatParam"), HeatParam);
}

void AShotgun::FireShot(FVector TowardsLocation)
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
		
		HeatParam += HeatAccumulationScale;

		//const FName TraceTag("Debug Trace");
		FHitResult result;
		ECollisionChannel collisionChannel;
		collisionChannel = ECC_WorldDynamic;
		FCollisionQueryParams collisionQuery;
		//collisionQuery.TraceTag = TraceTag;
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		collisionQuery.bTraceComplex = true;
		FCollisionObjectQueryParams objectCollisionQuery;
		objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
		FCollisionResponseParams collisionResponse;
		collisionResponse = ECR_Block;
		collisionQuery.AddIgnoredActor(this);
		collisionQuery.AddIgnoredActor(GetOwner());

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzeFlash, MuzzleFlashLocation->GetComponentLocation(), FRotator::ZeroRotator, true);
			FTransform particleTransform = particleComp->GetRelativeTransform();
			particleTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));
			particleComp->SetRelativeTransform(particleTransform);
		}

		for (int i = 0; i < NumberOfShots; i++)
		{

			float Step = (360.f / (float)NumberOfShots);
			float AngleMin = i * Step;
			float AngleMax = (i + 1) * Step;

			float Angle = FMath::FRandRange(AngleMin, AngleMax);
			float Radius = FMath::FRandRange(RadiusMin, RadiusMax);

			float Horizontal = Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
			float Vertical = Radius * FMath::Sin(FMath::DegreesToRadians(Angle));

			FVector DirectionVector = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
			DirectionVector.Normalize();

			FVector FinalDirection;

			FVector Forward = DirectionVector;
			Forward.Normalize();
			FVector WorldUp = FVector(0, 0, 1);
			FVector Left = FVector::CrossProduct(Forward, WorldUp);
			Left.Normalize();
			FVector LocalUp = FVector::CrossProduct(Forward, Left);
			LocalUp.Normalize();
			FVector New = Forward.RotateAngleAxis(Horizontal, LocalUp);
			New.Normalize();
			FinalDirection = New;

			Forward = FinalDirection;
			Forward.Normalize();
			WorldUp = FVector(0, 0, 1);
			Left = FVector::CrossProduct(Forward, WorldUp);
			Left.Normalize();
			LocalUp = FVector::CrossProduct(Forward, Left);
			LocalUp.Normalize();
			New = Forward.RotateAngleAxis(Vertical, Left);
			New.Normalize();
			FinalDirection = New * Distance;

			bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), BulletSpawnLocation->GetComponentLocation() + FinalDirection, collisionChannel, collisionQuery, collisionResponse);

			if (hitObject)
			{
				OnWeaponHit(result);

				if (result.GetActor() != NULL)
				{
					if (Cast<AEnemyCharacter>(result.GetActor()))
					{
						AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(result.GetActor());
						TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
						FDamageEvent DamageEvent(ValidDamageTypeClass);
						Enemy->TakeDamage(WeaponDamage * (1.0f - FMath::Clamp(result.Distance/Distance, 0.0f, 1.0f)), DamageEvent, result.GetActor()->GetInstigatorController(), this);
						
						FVector HitAngle = (TowardsLocation - BulletSpawnLocation->GetComponentLocation());
						HitAngle.Normalize();
						Enemy->AddDelayedImpulse(HitAngle * ImpulsePowah, result.Location);
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
}

void AShotgun::OnWeaponHit_Implementation(FHitResult HitResult)
{
	if (Cast<AEnemyCharacter>(HitResult.GetActor()))
	{
		// Blood splat particles
		if (BloodParticle != NULL)
		{
			UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(BloodParticle, HitResult.GetComponent(), TEXT("None"), HitResult.Location, HitResult.Normal.Rotation() * -1.f, EAttachLocation::KeepWorldPosition);
			ParticleSystemComp->AddLocalRotation(FRotator(90.f, 0.f, 0.f));
		}

		// Blood decals
		if (BloodDecal != NULL)
		{
			FHitResult result;
			ECollisionChannel collisionChannel;
			collisionChannel = ECC_WorldDynamic;
			FCollisionQueryParams collisionQuery;
			collisionQuery.bTraceComplex = true;
			FCollisionObjectQueryParams objectCollisionQuery;
			FCollisionResponseParams collisionResponse;
			collisionResponse = ECR_Block;

			collisionQuery.AddIgnoredActor(this);
			collisionQuery.AddIgnoredActor(GetOwner());
			collisionQuery.AddIgnoredActor(HitResult.GetActor());

			bool hitObject = GetWorld()->LineTraceSingleByChannel(result, HitResult.Location, HitResult.Location + FVector(0.f, 0.f, -1000.f), collisionChannel, collisionQuery, collisionResponse);

			if (hitObject)
			{
				float DecalSize = (BloodDecalMaxSize - BloodDecalMinSize) * FMath::FRand() + BloodDecalMinSize;
				UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BloodDecal, FVector(DecalSize, DecalSize, 1.f), result.Location, result.Normal.Rotation() * -1.f);
				DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
			}
		}
	}
	else if (Cast<UDestructibleComponent>(HitResult.GetActor()))
	{
		// Apply Damage to the component
		Cast<UDestructibleComponent>(HitResult.GetActor())->ApplyRadiusDamage(1.f, HitResult.Location, 1.f, ImpulsePowah, false);
	}
	else
	{
		// Spawn Decals
		if (BulletDecal != NULL)
		{
			float DecalSize = (MaxDecalSize - MinDecalSize) * FMath::FRand() + MinDecalSize;
			UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(BulletDecal, FVector(DecalSize, DecalSize, 1.f), HitResult.GetComponent(), TEXT("None"), HitResult.Location, HitResult.Normal.Rotation(), EAttachLocation::KeepWorldPosition);
			DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
		}
	}
}
