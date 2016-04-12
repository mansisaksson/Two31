#include "Two31.h"
#include "CultistCharacter.h"
#include "../Utilities/Weapon.h"
#include "PlayerCharacter.h"

ACultistCharacter::ACultistCharacter()
	: AEnemyCharacter()
{
	TimeToLooseLineOfSight = 1.f;
	TimeSinceSeenPlayer = 0;

	TurnRate = 100.f;

	AmmoPool = 2147483647; // Sätter ammo till max så att den inte tar slut
	bHasLineOfSight = false;
	bCanSeePlayerChest = false;
	bCanSeePlayerShoulder_Left = false;
	bCanSeePlayerShoulder_Right = false;

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

	if (bIsAlive)
	{
		CheckLineOfSight();
		if (bHasLineOfSight)
		{
			FireTowardsPlayer();
			ReactToPlayerMovement(DeltaTime);
			FocusOnPlayer(DeltaTime);
		}
		else
		{
			//TryGetLineOfSight();
		}
	}
}

void ACultistCharacter::CheckLineOfSight()
{
	FHitResult result;
	ECollisionChannel collisionChannel;
	collisionChannel = ECC_WorldDynamic;
	FCollisionQueryParams collisionQuery;
	collisionQuery.bTraceComplex = true;
	FCollisionObjectQueryParams objectCollisionQuery;
	objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	FCollisionResponseParams collisionResponse;
	collisionResponse = ECR_Block;
	collisionQuery.AddIgnoredActor(this);
	collisionQuery.AddIgnoredActor(CurrentWeapon);

	for (size_t i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Chest()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				bCanSeePlayerChest = true;
			else
				bCanSeePlayerChest = false;
		}
		else if (i == 1)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Left()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				bCanSeePlayerShoulder_Left = true;
			else
				bCanSeePlayerShoulder_Left = false;
		}
		else if (i == 2)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Right()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				bCanSeePlayerShoulder_Right = true;
			else
				bCanSeePlayerShoulder_Right = false;
		}
	}

	if (bCanSeePlayerChest || bCanSeePlayerShoulder_Left || bCanSeePlayerShoulder_Right)
		bHasLineOfSight = true;
	else if (TimeSinceSeenPlayer > TimeToLooseLineOfSight)
		bHasLineOfSight = false;

	//if (bCanSeePlayerChest)
	//	UE_LOG(DebugLog, Log, TEXT("Can See Chest"));
	//if (bCanSeePlayerShoulder_Left)
	//	UE_LOG(DebugLog, Log, TEXT("Can See PlayerShoulder_Left"));
	//if (bCanSeePlayerShoulder_Right)
	//	UE_LOG(DebugLog, Log, TEXT("Can See PlayerShoulder_Right"));
}

void ACultistCharacter::FireTowardsPlayer()
{
	if (PlayerReferense != NULL && CurrentWeapon != NULL)
	{
		FVector Direction = PlayerReferense->GetActorLocation() - GetActorLocation();

		//CurrentWeapon->StartFire(Direction * 5000.f);
		//CurrentWeapon->StopFire(Direction * 5000.f);
	}
}

void ACultistCharacter::FocusOnPlayer(float DeltaTime)
{
	if (PlayerReferense != NULL)
	{
		FVector Direction = PlayerReferense->GetActorLocation() - GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();

		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
		FaceRotation(NewControlRotation);
	}
}

void ACultistCharacter::ReactToPlayerMovement(float DeltaTime)
{
	if (bCanSeePlayerChest && bCanSeePlayerShoulder_Left && bCanSeePlayerShoulder_Right)
	{
		// No need to move as of now
	}
	else if (bCanSeePlayerShoulder_Left && !bCanSeePlayerShoulder_Right)
		AddMovementInput(GetActorRightVector(), DeltaTime * 50.f);
	else if (bCanSeePlayerShoulder_Right && !bCanSeePlayerShoulder_Left)
		AddMovementInput(-GetActorRightVector(), DeltaTime * 50.f);
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

		CurrentWeapon->EquipWeapon(GetMesh(), &AmmoPool);
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