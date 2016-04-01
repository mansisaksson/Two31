#include "Two31.h"
#include "Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsFiring = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
	WeaponMesh->SetOnlyOwnerSee(true);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	
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
}

void AWeapon::StartFire(FVector TowardsLocation)
{
	bIsFiring = true;
}

// Is called from the PlayerCharacter
void AWeapon::UpdateFire(FVector TowardsLocation)
{

}

void AWeapon::EndFire(FVector TowardsLocation)
{
	bIsFiring = false;
}

