#include "Two31.h"
#include "Weapon.h"
#include "Engine.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CultistCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ClipSize = 30;
	RPM = 500;

	ReloadTime = 2.f;
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

	MinDecalSize = 20.f;
	MaxDecalSize = 40.f;
	BloodDecalMinSize = 40.f;
	BloodDecalMaxSize = 100.f;

	bEquip = false;
	bIsEquiped = false;
	bReload = false;
	bFireIsPressed = false;
	bCanFire = true;
	bReadyToFire = false;
	bFirstTimeEquiped = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	RootComponent = WeaponMesh;

	BulletSpawnLocation = CreateDefaultSubobject<USceneComponent>("BulletSpawnLocation");
	BulletSpawnLocation->AttachTo(WeaponMesh, TEXT("BulletSpawn"));

	//AutoReceiveInput() <--- G�r detta sen
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
				bReload = true;

			if (bReload && (*AmmoPool) > 0)
			{
				timeSinceReloadStart += DeltaTime;
				if (timeSinceReloadStart >= ReloadTime)
				{
					bReload = false;
					timeSinceReloadStart = 0.f;
					FillClip();
				}
			}
		}
	}
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
		bReload = true;
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
	else
	{
		if (Cast<UDestructibleComponent>(HitResult.GetActor()))
			Cast<UDestructibleComponent>(HitResult.GetActor())->ApplyRadiusDamage(1.f, HitResult.Location, 1.f, ImpulsePowah, false);

		UMaterialInterface* DecalMat = NULL;
		UParticleSystem* ImpactParticle = NULL;

		if (HitResult.PhysMaterial != NULL)
		{
			if (HitResult.PhysMaterial->GetName() == "PM_Metal")
			{
				DecalMat = ImpactVisuals.MetalImpactDecal;
				ImpactParticle = ImpactVisuals.MetalImpactParticle;
			}
			else if (HitResult.PhysMaterial->GetName() == "PM_Wood")
			{
				DecalMat = ImpactVisuals.WoodImpactDecal;
				ImpactParticle = ImpactVisuals.WoodImpactParticle;
			}
			else
			{
				DecalMat = ImpactVisuals.DefaultImpactDecal;
				ImpactParticle = ImpactVisuals.DefaultImpactParticle;
			}
		}
		else
		{
			DecalMat = ImpactVisuals.DefaultImpactDecal;
			ImpactParticle = ImpactVisuals.DefaultImpactParticle;
		}

		if (DecalMat != NULL)
		{
			float DecalSize = (MaxDecalSize - MinDecalSize) * FMath::FRand() + MinDecalSize;
			UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(DecalMat, FVector(DecalSize, DecalSize, 1.f), HitResult.GetComponent(), TEXT("None"), HitResult.Location, HitResult.Normal.Rotation(), EAttachLocation::KeepWorldPosition);
			DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
		}

		if (ImpactParticle != NULL)
			UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, HitResult.Normal.Rotation());
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