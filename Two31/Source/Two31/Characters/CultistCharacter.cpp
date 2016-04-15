#include "Two31.h"
#include "CultistCharacter.h"
#include "../Utilities/Weapon.h"
#include "PlayerCharacter.h"

ACultistCharacter::ACultistCharacter()
	: AEnemyCharacter()
{
	TurnRate = 100.f;
	TimeToIdle = 20.f;

	AmmoPool = 2147483647; // Sätter ammo till max så att den inte tar slut
	bHasLineOfSight = false;
	TimeSinceLostLineOfSight = 0.f;

	WeaponSlots.SetNum(1);
	SpottedPlayerPositions.SetNum(5);
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
		if (EnemyState == EEnemyState::Triggered)
		{
			CheckLineOfSight();

			if (bHasLineOfSight)
			{
				if (PlayerReferense != NULL)
					FocusOnPosition(PlayerReferense->GetActorLocation());

				FireTowardsPlayer();
				ReactToPlayerMovement(DeltaTime);
			}
			else
			{
				TimeSinceLostLineOfSight += DeltaTime;
				if (TimeSinceLostLineOfSight >= TimeToIdle)
				{
					if (TimeSinceLostLineOfSight < 1.f)
						TryGetLineOfSight();
					else
						GuardLastKnownPosition();
				}
				else
				{
					// Go Idle
				}
			}
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

	SpottedPositions spottedPositions;
	for (size_t i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Chest()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				spottedPositions.bCanSeePlayerChest = true;
		}
		else if (i == 1)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Left()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				spottedPositions.bCanSeePlayerShoulder_Left = true;
		}
		else if (i == 2)
		{
			GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Right()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
			if (Cast<APlayerCharacter>(result.GetActor()))
				spottedPositions.bCanSeePlayerShoulder_Right = true;
		}
	}

	if (spottedPositions.bCanSeePlayerChest || spottedPositions.bCanSeePlayerShoulder_Left || spottedPositions.bCanSeePlayerShoulder_Right)
	{
		bHasLineOfSight = true;
		TimeSinceLostLineOfSight = true;
	}
	else
		bHasLineOfSight = false;

	SpottedPlayerPositions.RemoveAt(0);
	SpottedPlayerPositions.Add(spottedPositions);
}

void ACultistCharacter::FireTowardsPlayer()
{
	if (PlayerReferense != NULL && CurrentWeapon != NULL)
	{
		FVector Direction = (PlayerReferense->GetActorLocation() + FVector(FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f))) - GetActorLocation();

		CurrentWeapon->StartFire(Direction * 5000.f);
		CurrentWeapon->StopFire(Direction * 5000.f);
	}
}
void ACultistCharacter::FocusOnPosition(FVector Position)
{
	FVector Direction = Position - GetActorLocation();
	FRotator NewControlRotation = Direction.Rotation();

	// Gör en lerp här
	NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
	FaceRotation(NewControlRotation);
}
void ACultistCharacter::ReactToPlayerMovement(float DeltaTime)
{
	// if (player is getting too close)
		AvoidPlayer();
	
	if (SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Left && !SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Right)
		AddMovementInput(GetActorRightVector(), DeltaTime * 50.f);
	else if (SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Right && !SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Left)
		AddMovementInput(-GetActorRightVector(), DeltaTime * 50.f);

	// Save Player Position
}
void ACultistCharacter::AvoidPlayer()
{
	//	Try To move away from enemy:
	//		Try to move back
	//		else: Try to move left
	//		else: Try to move right
	//		else: Stand Still and shoot player
}

void ACultistCharacter::TryGetLineOfSight()
{
	// Sidestep to the most recent know position
}
void ACultistCharacter::GuardLastKnownPosition()
{
	if (true)
		GoCloseToLastKnowPosition();
	else
	{
		// To till en random position runt sig
		// FaceLocation(last knownLocation)

		// if (x time has passed)
		//		move too location(spawn location)
		//		go idle;
	}
}
void ACultistCharacter::GoCloseToLastKnowPosition()
{
	// gå nära den senaste kända positionen av spelaren
}

void ACultistCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	
}
void ACultistCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (Cast<APlayerCharacter>(OtherPawn))
		EnemyState = EEnemyState::Triggered;
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