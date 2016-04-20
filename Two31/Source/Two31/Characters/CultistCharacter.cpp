#include "Two31.h"
#include "CultistCharacter.h"
#include "../Utilities/Weapon.h"
#include "PlayerCharacter.h"

ACultistCharacter::ACultistCharacter()
	: AEnemyCharacter()
{
	AmmoPool = 2147483647; // Sätter ammo till max så att den inte tar slut

	TurnRate = 10.f;
	TimeToIdle = 20.f;

	AvoidDamage = 20.f;
	TimeToRandMove = 4.0f;
	AddedRandomTime = 4.f;
	TimeGivenToMove = 1.5f;
	TimeToMoveTooLastKnownPosititon = 5.f;

	bPrioritizeLineOfSight = false;
	bInstaReactToCornerPeaking = false;

	MinRandMoveRadius = 300.f;
	MaxRandMoveRadius = 400.f;

	RetreatDistance = 500.f;
	HuntDistance = 1500.f;

	bHasLineOfSight = false;
	bRandMoveAwayFromPlayer = false;
	TimeSinceLostLineOfSight = 0.f;
	TimeSinceRandMovement = 0.f;
	
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
	DefaultTimeToRandMove = TimeToRandMove;
	TimeToRandMove = DefaultTimeToRandMove + FMath::FRandRange(0.f, AddedRandomTime);

	if (PlayerReferense == NULL)
		Debug::LogFatalError("Could not find Player Character!");
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
					FocusOnPosition(DeltaTime, PlayerReferense->GetActorLocation());

				FireTowardsPlayer(DeltaTime);
				ReactToPlayerMovement(DeltaTime);
			}
			else
			{
				TimeSinceLostLineOfSight += DeltaTime;
				if (TimeSinceLostLineOfSight < TimeToIdle)
				{
					if (bOldHasLineOfSight && !bHasLineOfSight)
						bLostLineOfSight = true;
					if (TimeSinceLostLineOfSight > 1.f && TimeSinceLostLineOfSight < 3.f)
					{
						TimeSinceRanToLastKnownPosition = 0.f;
						TryGetLineOfSight(DeltaTime);
					}
					else
						GuardLastKnownPosition(DeltaTime);
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
	if (PlayerReferense != NULL)
	{
		if (PlayerReferense->GetPlayerHitPoint_Chest() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Left() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Right() == NULL)
			return;//Debug::LogFatalError("Hit Points on player is NULL");

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

		bOldHasLineOfSight = bHasLineOfSight;
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
}

void ACultistCharacter::FireTowardsPlayer(float DeltaTime)
{
	if (PlayerReferense != NULL && CurrentWeapon != NULL)
	{
		FVector Direction = (PlayerReferense->GetActorLocation() + FVector(FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f))) - GetActorLocation();

		// Gör detta i intervall, möjligtvis olika beroende på vapen
		CurrentWeapon->StartFire(Direction * 5000.f);
		CurrentWeapon->StopFire(Direction * 5000.f);
	}
}
void ACultistCharacter::FocusOnPosition(float DeltaTime, FVector Position)
{
	FVector Direction = Position - GetActorLocation();
	FRotator NewControlRotation = Direction.Rotation();

	// Gör en lerp här
	NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
	//FaceRotation(FMath::Lerp(GetActorRotation(), NewControlRotation, DeltaTime * TurnRate));
	FaceRotation(NewControlRotation);
}
void ACultistCharacter::ReactToPlayerMovement(float DeltaTime)
{
	// Notes för framtiden:
	// Kolla om fienden är åvanför spelaren och agera anorlunda utifrån detta
	float distanceToPlayer = FVector::Dist(GetActorLocation(), PlayerReferense->GetActorLocation());
	if (distanceToPlayer < 500.f)
		AvoidPlayer(DeltaTime);
	//else if (distanceToPlayer > HuntDistance)
	//	HuntPlayer();

	TimeSinceRandMovement += DeltaTime;
	if (TimeSinceRandMovement > TimeToRandMove)
	{
		TimeToRandMove = DefaultTimeToRandMove + FMath::FRandRange(0.f, AddedRandomTime);
		TimeSinceRandMovement = 0.f;

		FVector NewPos;
		if (bRandMoveAwayFromPlayer)
		{
			bRandMoveAwayFromPlayer = false;
			float RandRad = FMath::FRandRange(MinRandMoveRadius, MaxRandMoveRadius);
			float RandDir = FMath::FRandRange(-90.f, 90.f);
			NewPos = GetActorLocation();
			NewPos += GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * -RandRad;
		}
		else
		{
			float RandRad = FMath::FRandRange(MinRandMoveRadius, MaxRandMoveRadius);
			float RandDir = FMath::FRandRange(0.f, 360.f);
			NewPos = GetActorLocation();
			NewPos += GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * RandRad;
		}
		
		NavSystem->SimpleMoveToLocation(GetController(), NewPos);
		bIsRandMoving = true;
	}
	if (TimeSinceRandMovement > TimeGivenToMove)
		bIsRandMoving = false;

	if (bInstaReactToCornerPeaking && (!bIsRandMoving || bPrioritizeLineOfSight))
	{
		if (SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Left && !SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Right)
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * 100.f);
		else if (SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Right && !SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Left)
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * -100.f);
	}
	LastKnownPlayerPos = PlayerReferense->GetActorLocation();
}
void ACultistCharacter::AvoidPlayer(float DeltaTime)
{
	const FName TraceTag("Debug Trace");
	FHitResult result;
	ECollisionChannel collisionChannel;
	collisionChannel = ECC_WorldDynamic;
	FCollisionQueryParams collisionQuery;
	collisionQuery.TraceTag = TraceTag;
	GetWorld()->DebugDrawTraceTag = TraceTag;
	collisionQuery.bTraceComplex = true;
	FCollisionObjectQueryParams objectCollisionQuery;
	objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	FCollisionResponseParams collisionResponse;
	collisionResponse = ECR_Block;
	collisionQuery.AddIgnoredActor(this);

	float MoveSpeed = 80.f;
	float NavRadius = 100.f;

	if (!bIsRandMoving)
	{
		// If Can Move Backwards
		if (!GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * -NavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorForwardVector() * -MoveSpeed);
		// Move Left If needed
		if (GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorRightVector() * -NavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * MoveSpeed);
		// Move Right If needed
		if (GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorRightVector() * NavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * -MoveSpeed);
	}
}

void ACultistCharacter::TryGetLineOfSight(float DeltaTime)
{
	FocusOnPosition(DeltaTime, LastKnownPlayerPos);
	// If We just lost line of sight
	if (bLostLineOfSight)
	{
		// Find the most recent known angle of the player
		for (size_t i = SpottedPlayerPositions.Num() - 1; i > 0; i--)
		{
			if (SpottedPlayerPositions[i].bCanSeePlayerShoulder_Left || SpottedPlayerPositions[i].bCanSeePlayerShoulder_Right || SpottedPlayerPositions[i].bCanSeePlayerChest)
				LastKnownSpottedPositions = SpottedPlayerPositions[i];
		}
		bLostLineOfSight = false;
	}

	// Sidestep to the most recent know position
	if (LastKnownSpottedPositions.bCanSeePlayerShoulder_Left && !LastKnownSpottedPositions.bCanSeePlayerShoulder_Right)
		NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * 100.f));
	else if (LastKnownSpottedPositions.bCanSeePlayerShoulder_Right && !LastKnownSpottedPositions.bCanSeePlayerShoulder_Left)
		NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * -100.f));
}
void ACultistCharacter::GuardLastKnownPosition(float DeltaTime)
{
	if (TimeSinceRanToLastKnownPosition == 0.f)
	{
		Debug::LogOnScreen("Go Close to location");
		NavSystem->SimpleMoveToLocation(GetController(), PlayerReferense->GetActorLocation());
	}
	else if (TimeSinceRanToLastKnownPosition > TimeToMoveTooLastKnownPosititon)
	{
		Debug::LogOnScreen("Done Moving");
		// Gå till en random position runt sig
		// FaceLocation(last knownLocation)

		// if (x time has passed)
		//		move too location(spawn location)
		//		go idle;

		/*FNavLocation NewPos;
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), MaxRandMoveRadius, NewPos);
		NavSystem->SimpleMoveToLocation(GetController(), NewPos.Location);*/
	}
	TimeSinceRanToLastKnownPosition += DeltaTime;
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
float ACultistCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	Debug::LogOnScreen(FString::Printf(TEXT("Current Health: %f"), CurrentHealth));

	TimeSinceRandMovement += TimeToRandMove * (AvoidDamage / 100.f);
	if (TimeSinceRandMovement >= TimeToRandMove)
		bRandMoveAwayFromPlayer = true;

	if (CurrentHealth == 0)
		bIsAlive = false;
	return DamageAmount;
}