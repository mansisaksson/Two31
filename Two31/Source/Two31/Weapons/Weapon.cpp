#include "Two31.h"
#include "Weapon.h"
#include "Engine.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"
//#include "../Interactables/Confetti.h"
#include "../StatsPornManager.h"
#include "../BloodParticleBall.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponName = "Gun";

	ClipSize = 30;
	RPM = 500;

	FullReloadTime = 2.f;
	FastReloadTime = 1.5f;
	EquipTime = 1.f;
	UnequipTime = 1.f;

	bAutoReload = true;

	timeSinceFire = 0.f;
	timeSinceReloadStart = 0.f;
	timeSinceEquip = 0.f;
	timeSinceUnequip = 0.f;

	WeaponDamage = 10;
	Range = 5000.f;
	HeadshotMultiplier = 1.0f;
	ImpulsePowah = 9000.f;

	bEquip = false;
	bIsEquiped = false;
	bReload = false;
	bFireIsPressed = false;
	bCanFire = true;
	bReadyToFire = false;
	bFirstTimeEquiped = true;

	HipTransform.SetLocation(FVector(3.160810f, -9.322928f, -157.550308f));
	HipTransform.SetRotation(FRotator(-1.093220f, -108.702850f, -0.264890f).Quaternion());
	HipTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	ADSTransform.SetLocation(FVector(0.f, -30.65f, -148.2f));
	ADSTransform.SetRotation(FRotator(3.f, -108.2f, 0.f).Quaternion());
	ADSTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	RootComponent = WeaponMesh;

	BulletSpawnLocation = CreateDefaultSubobject<USceneComponent>("BulletSpawnLocation");
	BulletSpawnLocation->SetupAttachment(WeaponMesh, TEXT("BulletSpawn"));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEquip)
	{
		timeSinceEquip += DeltaTime;
		if (timeSinceEquip >= EquipTime)
		{
			timeSinceEquip = 0.f;
			bEquip = false;
			bIsEquiped = true;
		}
	}

	if (bIsEquiped)
	{
		if (AmmoPool == NULL)
			return;

		if (!bCanFire && !bReadyToFire)
		{
			timeSinceFire += DeltaTime;
			if (timeSinceFire > 1.f / (RPM / 60.f))
			{
				timeSinceFire = 0;
				bReadyToFire = true;
			}
		}

		if (bReadyToFire && (bFullAuto || !bFireIsPressed))
		{
			bCanFire = true;
			bReadyToFire = false;
		}

		if (bCanFire && (!bFireIsPressed || timeSinceReloadStart > 0.f))
		{
			if (bAutoReload && AmmoInClip <= 0)
			{
				TotalReloadTime = FullReloadTime;
				bReload = true;
			}

			if (bReload && (*AmmoPool) > 0)
			{
				timeSinceReloadStart += DeltaTime;
				if (timeSinceReloadStart >= TotalReloadTime)
				{
					bReload = false;
					timeSinceReloadStart = 0.f;
					FillClip();
				}
			}
		}
	}
}

FString AWeapon::GetWeaponName()
{
	return WeaponName;
}

void AWeapon::EquipWeapon(USkeletalMeshComponent* SkeletalMesh, int* ammoPool)
{
	if (Cast<APlayerCharacter>(SkeletalMesh->GetAttachmentRootActor()))
	{
		SetPlayerAnimations(SkeletalMesh);
		WeaponMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		WeaponMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
	else if (Cast<ACultistCharacter>(SkeletalMesh->GetAttachmentRootActor()))
	{
		SetCultistOwnerMesh(SkeletalMesh);
		WeaponMesh->SetRelativeRotation(CultistRotOffset);
		WeaponMesh->SetRelativeLocation(CultistLocOffset);
		//WeaponMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		//WeaponMesh->SetRelativeLocation(FVector(-9.f, 5.f, -2.f));
	}

	SetAmmoPool(ammoPool);

	if (bFirstTimeEquiped)
		FillClip();

	bEquip = true;
	bIsEquiped = false;
	bFirstTimeEquiped = false;

	timeSinceEquip = 0.f;
	timeSinceUnequip = 0.f;
	timeSinceReloadStart = 0.f;

	SetActorHiddenInGame(false);
}
void AWeapon::HolsterWeapon()
{
	bEquip = false;
	bIsEquiped = false;
	bReload = false;

	timeSinceEquip = 0.f;
	timeSinceUnequip = 0.f;
	timeSinceReloadStart = 0.f;

	SetActorHiddenInGame(true);
}

void AWeapon::SetPlayerAnimations(USkeletalMeshComponent* PlayerMesh)
{
	OwnerMesh = PlayerMesh;
	if (PlayerAnimationBlueprint != NULL && OwnerMesh != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Changing Player Animations")));
		OwnerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		OwnerMesh->SetAnimInstanceClass(PlayerAnimationBlueprint);
	}
}
void AWeapon::SetCultistOwnerMesh(USkeletalMeshComponent* CultistMesh)
{
	OwnerMesh = CultistMesh;
}

void AWeapon::StartFire(FVector TowardsLocation)
{
	if (bIsEquiped)
	{
		if (bCanFire && !bReload)
		{
			OnBeginFire();
			bCanFire = false;
			bFireIsPressed = true;
			timeSinceFire = 0.f;
			FireShot(TowardsLocation);
		}
	}
}
void AWeapon::UpdateFire(FVector TowardsLocation)
{
	if (bIsEquiped)
	{
		if (bCanFire && bFullAuto && !bReload)
		{
			OnUpdateFire();
			bCanFire = false;
			bFireIsPressed = true;
			timeSinceFire = 0.f;
			FireShot(TowardsLocation);
		}
	}
}
void AWeapon::StopFire(FVector TowardsLocation)
{
	OnStopFire();
	bFireIsPressed = false;
}
void AWeapon::FireShot(FVector TowardsLocation)
{
	if (GetOwner() != NULL)
	{
		if (Cast<APlayerCharacter>(GetOwner()))
			UStatsPornManager::IncreaseAmountOfShotsFired();
	}
}

void AWeapon::Reload()
{
	if (AmmoPool != NULL)
	{
		if ((AmmoInClip < ClipSize) && (*AmmoPool - AmmoInClip) > 0)
		{
			if (AmmoInClip == 0)
				TotalReloadTime = FullReloadTime;
			else
				TotalReloadTime = FastReloadTime;
			bReload = true;
		}
	}
}
void AWeapon::SetAmmoPool(int* ammoPool)
{
	AmmoPool = ammoPool;

	if (bFirstTimeEquiped && ammoPool != NULL)
		FillClip();
}
void AWeapon::FillClip()
{
	if (AmmoPool != NULL)
	{
		int AmmoDelta = (*AmmoPool) - ClipSize;

		if (AmmoDelta >= 0)
			AmmoInClip = ClipSize;
		else
			AmmoInClip = ClipSize + AmmoDelta;
	}
}

AActor* AWeapon::GetOwner()
{
	if (OwnerMesh != NULL)
		return OwnerMesh->GetAttachmentRootActor();
	return NULL;
}

void AWeapon::OnWeaponHit_Implementation(FHitResult HitResult)
{

	if (HitResult.GetActor() != NULL)
	{

		// Deal Damage
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		if (HitResult.BoneName.GetPlainNameString() == "Head")
			HitResult.GetActor()->TakeDamage((WeaponDamage * (1.0f - FMath::Clamp(HitResult.Distance / Range, 0.0f, 1.0f)) * HeadshotMultiplier), DamageEvent, HitResult.GetActor()->GetInstigatorController(), this);
		else
			HitResult.GetActor()->TakeDamage(WeaponDamage * (1.0f - FMath::Clamp(HitResult.Distance / Range, 0.0f, 1.0f)), DamageEvent, HitResult.GetActor()->GetInstigatorController(), this);

		// Add Impulses
		if (Cast<AEnemyCharacter>(HitResult.GetActor()))
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitResult.GetActor());
			FVector HitAngle = (HitResult.Location - BulletSpawnLocation->GetComponentLocation());
			HitAngle.Normalize();
			Enemy->AddDelayedImpulse(HitAngle * ImpulsePowah, HitResult.Location);

			Enemy->SpawnBloodEffects(HitResult, this);
		}
		else if (HitResult.GetComponent() != NULL && HitResult.GetComponent()->Mobility == EComponentMobility::Movable && HitResult.GetComponent()->IsSimulatingPhysics())
		{
			FVector Angle = (HitResult.Location - BulletSpawnLocation->GetComponentLocation());
			Angle.Normalize();
			HitResult.GetComponent()->AddImpulseAtLocation(Angle * ImpulsePowah, HitResult.Location);
		}
	}

	// Destroy Destructibles
	if (Cast<UDestructibleComponent>(HitResult.GetActor()))
		Cast<UDestructibleComponent>(HitResult.GetActor())->ApplyRadiusDamage(1.f, HitResult.Location, 1.f, ImpulsePowah, false);


	// Decals/Impact effects
	UMaterialInterface* DecalMat = NULL;
	UParticleSystem* ImpactParticle = NULL;
	FVector ParticleScale;

	float RandSize = 0.f;
	FVector DecalSize = FVector::ZeroVector;

	if (HitResult.PhysMaterial != NULL)
	{
		for (int32 i = 0; i < ImpactVisuals.Num(); i++)
		{
			if (HitResult.PhysMaterial->GetName() == ImpactVisuals[i].PhysicsMatName)
			{
				ImpactParticle = ImpactVisuals[i].ImpactParticle;
				ParticleScale = ImpactVisuals[i].ParticleScale;
				if (ImpactVisuals[i].ImpactDecals.Num() > 0)
				{
					int index = FMath::RandRange(0, ImpactVisuals[i].ImpactDecals.Num() - 1);
					DecalMat = ImpactVisuals[i].ImpactDecals[index].Decal;
					DecalSize = ImpactVisuals[i].ImpactDecals[index].Size;
					RandSize = FMath::RandRange(0.f, ImpactVisuals[i].ImpactDecals[index].AddedRandXYSize);
				}
			}
		}
	}

	if (DecalMat == NULL)
	{
		for (int32 i = 0; i < ImpactVisuals.Num(); i++)
		{
			if (ImpactVisuals[i].PhysicsMatName == "Default")
			{
				if (ImpactVisuals[i].ImpactDecals.Num() > 0)
				{
					int index = FMath::RandRange(0, ImpactVisuals[i].ImpactDecals.Num() - 1);
					DecalMat = ImpactVisuals[i].ImpactDecals[index].Decal;
					DecalSize = ImpactVisuals[i].ImpactDecals[index].Size;
					RandSize = FMath::RandRange(0.f, ImpactVisuals[i].ImpactDecals[index].AddedRandXYSize);
				}
				break;
			}
		}
	}
	if (ImpactParticle == NULL)
	{
		for (int32 i = 0; i < ImpactVisuals.Num(); i++)
		{
			if (ImpactVisuals[i].PhysicsMatName == "Default")
			{
				ImpactParticle = ImpactVisuals[i].ImpactParticle;
				ParticleScale = ImpactVisuals[i].ParticleScale;
				break;
			}
		}
	}

	if (!Cast<APlayerCharacter>(HitResult.GetActor()))
	{
		if (DecalMat != NULL)
		{
			FVector Normal = (HitResult.Normal * -1);
			UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(DecalMat, DecalSize, HitResult.GetComponent(), HitResult.BoneName, HitResult.Location, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition);
			if (DecalComp != NULL)
			{
				DecalComp->SetWorldRotation(Normal.Rotation());
				DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
				DecalComp->FadeScreenSize = 0.f;
			}
		}

		if (ImpactParticle != NULL)
		{
			UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, HitResult.Normal.Rotation());
			ParticleSystemComp->SetWorldScale3D(ParticleScale);
		}
	}
}
void AWeapon::OnBeginFire_Implementation()
{

}
void AWeapon::OnUpdateFire_Implementation()
{

}
void AWeapon::OnStopFire_Implementation()
{

}
