#include "Two31.h"
#include "Weapon.h"
#include "Engine.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

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
	BulletSpawnLocation->AttachTo(WeaponMesh, TEXT("BulletSpawn"));

	FImpactVisual DefaultImpactVisual;
	DefaultImpactVisual.PhysicsMatName = "Default";
	ImpactVisuals.Add(DefaultImpactVisual);
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

void AWeapon::EquipWeapon(USkeletalMeshComponent* SkeletalMesh, int* AmmoPool)
{
	if (Cast<APlayerCharacter>(SkeletalMesh->GetAttachmentRootActor()))
	{
		SetPlayerAnimations(SkeletalMesh);
		WeaponMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		WeaponMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
	else if (Cast<ACultistCharacter>(SkeletalMesh->GetAttachmentRootActor()))
	{
		SetCultistAnimations(SkeletalMesh);
		WeaponMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		WeaponMesh->SetRelativeLocation(FVector(-9.f, 5.f, -2.f));
	}

	SetAmmoPool(AmmoPool);

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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Changing Player Animations")));
		OwnerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		OwnerMesh->SetAnimInstanceClass(PlayerAnimationBlueprint);
	}
}
void AWeapon::SetCultistAnimations(USkeletalMeshComponent* CultistMesh)
{
	OwnerMesh = CultistMesh;
	if (PlayerAnimationBlueprint != NULL && OwnerMesh != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Changing Cultist Animations")));
		OwnerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		OwnerMesh->SetAnimInstanceClass(CultistAnimationBlueprint);
	}
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

}

void AWeapon::Reload()
{
	if (AmmoInClip < ClipSize)
	{
		if (AmmoInClip == 0)
			TotalReloadTime = FullReloadTime;
		else
			TotalReloadTime = FastReloadTime;
		bReload = true;
	}
		
}
void AWeapon::SetAmmoPool(int* AmmoPool)
{
	this->AmmoPool = AmmoPool;
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
	if (Cast<UDestructibleComponent>(HitResult.GetActor()))
		Cast<UDestructibleComponent>(HitResult.GetActor())->ApplyRadiusDamage(1.f, HitResult.Location, 1.f, ImpulsePowah, false);

	UMaterialInterface* DecalMat = NULL;
	UParticleSystem* ImpactParticle = NULL;

	float RandSize = 0.f;// = FMath::RandRange(0.f, DecalRandSize);
	FVector DecalSize = FVector::ZeroVector;// = FVector(DecalScale, DecalScale, 1.f);

	if (HitResult.PhysMaterial != NULL)
	{
		for (size_t i = 0; i < ImpactVisuals.Num(); i++)
		{
			if (HitResult.PhysMaterial->GetName() == ImpactVisuals[i].PhysicsMatName)
			{
				ImpactParticle = ImpactVisuals[i].ImpactParticle;
				if (ImpactVisuals[i].ImpactDecals.Num() > 0)
				{
					int index = FMath::RandRange(0, ImpactVisuals[i].ImpactDecals.Num() - 1);
					DecalMat = ImpactVisuals[i].ImpactDecals[index].Decal;
					DecalSize = ImpactVisuals[i].ImpactDecals[index].Size;
					RandSize = FMath::RandRange(0.f, ImpactVisuals[i].ImpactDecals[index].AddedRandXYSize);
				}
			}
		}

		if (DecalMat == NULL)
		{
			for (size_t i = 0; i < ImpactVisuals.Num(); i++)
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
			for (size_t i = 0; i < ImpactVisuals.Num(); i++)
			{
				if (ImpactVisuals[i].PhysicsMatName == "Default")
				{
					ImpactParticle = ImpactVisuals[i].ImpactParticle;
					break;
				}
			}
		}
	}

	if (DecalMat != NULL)
	{
		FVector Normal = (HitResult.Normal * -1);
		UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(DecalMat, DecalSize, HitResult.GetComponent(), HitResult.BoneName, HitResult.Location, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition);
		DecalComp->SetWorldRotation(Normal.Rotation());
		DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
	}

	if (ImpactParticle != NULL)
		UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, HitResult.Normal.Rotation());

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
