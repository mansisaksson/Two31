#include "Two31.h"
#include "CultistCharacter.h"
#include "../Utilities/Weapon.h"
#include "PlayerCharacter.h"

ACultistCharacter::ACultistCharacter()
	: AEnemyCharacter()
{
	bHasLineOfSight = false;
	WeaponSlots.SetNum(1);
}

void ACultistCharacter::BeginPlay()
{
	AEnemyCharacter::BeginPlay();
	EquipWeapon(Weapon);

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Cast<APlayerCharacter>(*Itr))
			PlayerReferense = Cast<APlayerCharacter>(*Itr);
	}
}

void ACultistCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);


	//AddMovementInput(GetActorForwardVector(), 10 * DeltaTime);

	// Look toward focus
	if (PlayerReferense != NULL)
	{
		FVector Direction = PlayerReferense->GetActorLocation() - GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();

		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
		FaceRotation(NewControlRotation, DeltaTime);
	}
}

void ACultistCharacter::TryGetLineOfSight()
{

}

void ACultistCharacter::UpdateLocationIfNeeded()
{

}

void ACultistCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	
}

void ACultistCharacter::OnSeePawn(APawn *OtherPawn)
{

}

bool ACultistCharacter::EquipWeapon(TSubclassOf<AWeapon> Weapon)
{
	if (Weapon != NULL)
	{
		for (size_t i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i] == NULL)
			{
				const FRotator SpawnRotation = FRotator::ZeroRotator;
				const FVector SpawnLocation = FVector::ZeroVector;
				WeaponSlots[i] = GetWorld()->SpawnActor<AWeapon>(Weapon, SpawnLocation, SpawnRotation);
				WeaponSlots[i]->AttachRootComponentTo(GetMesh(), TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
				WeaponSlots[i]->SetActorHiddenInGame(true);

				if (CurrentWeapon == NULL)
					SelectWeaponSlot(i);
				return true;
			}
		}
	}
	return false;
}

void ACultistCharacter::SelectWeaponSlot(int index)
{
	if (WeaponSlots[index] != NULL && CurrentWeapon != WeaponSlots[index])
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlots[index];
		CurrentWeapon->SetActorHiddenInGame(false);

		switch ((EAmmoType)CurrentWeapon->GetAmmoType())
		{
		case EAmmoType::BulletAmmo:
			CurrentAmmo = &BulletAmmo;
			break;
		case EAmmoType::ShotgunAmmo:
			CurrentAmmo = &ShotgunAmmo;
			break;
		case EAmmoType::ExplosiveAmmo:
			CurrentAmmo = &ExplosiveAmmo;
			break;
		case EAmmoType::PlasmaAmmo:
			CurrentAmmo = &PlasmaAmmo;
			break;
		default:
			CurrentAmmo = NULL;
			break;
		}

		if (CurrentAmmo != NULL)
			CurrentWeapon->EquipWeapon(GetMesh(), &CurrentAmmo->AmmoPool);
	}
}

int ACultistCharacter::GetWeaponIndex()
{
	int index = -1;
	for (size_t i = 0; i < WeaponSlots.Num(); i++)
	{
		if (CurrentWeapon == WeaponSlots[i])
			index = i;
	}
	return index;
}

void ACultistCharacter::Death()
{
	AEnemyCharacter::Death();
	for (size_t i = 0; i < WeaponSlots.Num(); i++)
	{
		if (WeaponSlots[i] != NULL)
			WeaponSlots[i]->Destroy();
	}
}