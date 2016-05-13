#include "Two31.h"
#include "Shotgun.h"
#include <iostream>
#include <sstream>
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"
#include "../StatsPornManager.h"
#include "Kismet/KismetMaterialLibrary.h"

AShotgun::AShotgun()
	: AWeapon()
{
	ClipSize = 4;
	RPM = 60;
	FullReloadTime = 2.f;
	FastReloadTime = 1.5f;
	timeSinceFire = 0;
	RadiusMin = 0.f;
	RadiusMax = 10.f;
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

		UStatsPornManager::IncreaseAmountOfShotsFired();
		
		HeatParam += HeatAccumulationScale;

		FHitResult result;
		ECollisionChannel collisionChannel;
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

		if (MuzzeFlash != NULL)
		{
			UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(MuzzeFlash, MuzzleFlashLocation);
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
			FinalDirection = New * Range;

			bool hitObject = GetWorld()->LineTraceSingleByChannel(result, BulletSpawnLocation->GetComponentLocation(), BulletSpawnLocation->GetComponentLocation() + FinalDirection, collisionChannel, collisionQuery, collisionResponse);
			
			if (hitObject)
				OnWeaponHit(result);
		}
	}
}
