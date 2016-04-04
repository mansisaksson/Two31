#include "Two31.h"
#include "Weapon.h"
#include "Engine.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ClipSize = 30;
	CurrentAmmo = 30;
	bIsFiring = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
	WeaponMesh->SetOnlyOwnerSee(false);
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

void AWeapon::EquipWeapon(USkeletalMeshComponent* ArmMesh, int* AmmoPool)
{
	SetArmAnimations(ArmMesh);
	SetAmmoPool(AmmoPool);
}

void AWeapon::SetArmAnimations(USkeletalMeshComponent* ArmMesh)
{
	this->ArmMesh = ArmMesh;
	if (ArmAnimationBlueprint != NULL && ArmMesh != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Changing Animations")));
		ArmMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		ArmMesh->SetAnimInstanceClass(ArmAnimationBlueprint);
	}
}

void AWeapon::StartFire(FVector TowardsLocation)
{
	bIsFiring = true;
}

void AWeapon::UpdateFire(float DeltaSeconds, FVector TowardsLocation)
{

}

void AWeapon::StopFire(FVector TowardsLocation)
{
	bIsFiring = false;
}

void AWeapon::SetAmmoPool(int* AmmoPool)
{
	this->AmmoPool = AmmoPool;
}

bool AWeapon::GetIsFiring()
{
	return bIsFiring;
}

int32 AWeapon::GetClipSize()
{
	return ClipSize;
}
int32 AWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}
uint8 AWeapon::GetAmmoType()
{
	return (uint8)AmmoType;
}