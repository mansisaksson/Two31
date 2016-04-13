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
	bAutoReload = true;

	timeSinceFire = 0.f;
	timeSinceReloadStart = 0.f;

	WeaponDamage = 10;

	bReload = false;
	bIsFiring = false;
	bCanFire = true;
	bReadyToFire = false;
	bFirstTimeEquiped = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	RootComponent = WeaponMesh;

	BulletSpawnLocation = CreateDefaultSubobject<USceneComponent>("BulletSpawnLocation");
	BulletSpawnLocation->AttachTo(WeaponMesh);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AmmoPool == NULL)
		return;

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

	if (!bCanFire)
	{
		timeSinceFire += DeltaTime;
		if (timeSinceFire > 1.f / (RPM / 60.f))
		{
			timeSinceFire = 0;
			bReadyToFire = true;
		}
	}

	if (bReadyToFire && (bFullAuto || (!bFullAuto && !bIsFiring)))
	{
		bCanFire = true;
		bReadyToFire = false;
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

	bFirstTimeEquiped = false;
	timeSinceReloadStart = 0.f;
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
	if (bCanFire && !bReload)
	{
		bCanFire = false;
		bIsFiring = true;
		timeSinceFire = 0.f;
		FireShot(TowardsLocation);
	}
}
void AWeapon::UpdateFire(FVector TowardsLocation)
{
	if (bCanFire && bFullAuto && !bReload)
	{
		bCanFire = false;
		bIsFiring = true;
		timeSinceFire = 0.f;
		FireShot(TowardsLocation);
	}
}
void AWeapon::StopFire(FVector TowardsLocation)
{
	bIsFiring = false;
}
void AWeapon::FireShot(FVector TowardsLocation)
{

}

void AWeapon::Reload()
{
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

bool AWeapon::GetIsFiring()
{
	return bIsFiring;
}

int32 AWeapon::GetClipSize()
{
	return ClipSize;
}
int32 AWeapon::GetAmmoInClip()
{
	return AmmoInClip;
}
uint8 AWeapon::GetAmmoType()
{
	return (uint8)AmmoType;
}

float AWeapon::GetTotalReloadTime()
{
	return ReloadTime;
}
float AWeapon::GetTimeSinceReload()
{
	return timeSinceReloadStart;
}
AActor* AWeapon::GetOwner()
{
	if (OwnerMesh != NULL)
		return OwnerMesh->GetAttachmentRootActor();
	return NULL;
}

void AWeapon::OnWeaponHit_Implementation(FHitResult HitResult)
{
	
}