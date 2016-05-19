#include "Two31.h"
#include "CultistCharacter.h"
#include "../Weapons/Weapon.h"
#include "PlayerCharacter.h"

ACultistCharacter::ACultistCharacter()
	: AEnemyCharacter()
{
	AmmoPool = 2147483647; // Sätter ammo till max så att den inte tar slut

	TurnRate = 10.f;
	TimeToIdle = 20.f;

	RPM = 480;
	MinBurstPause = 0.5f;
	MaxBurstPause = 2.f;
	MinBurstSize = 8;
	MaxBurstSize = 10;

	TriggeredMoveSpeed = 500.f;
	SearchMoveSpeed = 200.f;
	AvoidDamage = 20.f;
	TimeToRandMove = 4.0f;
	AddedRandomTime = 4.f;
	TimeGivenToMove = 1.5f;
	TimeToMoveTooLastKnownPosititon = 5.f;

	SearchTime = 20.f;

	bPrioritizeLineOfSight = false;
	bInstaReactToCornerPeaking = false;

	MinRandMoveRadius = 300.f;
	MaxRandMoveRadius = 400.f;

	RetreatDistance = 500.f;
	RetreatRadius = 80.f;
	RetreatNavRadius = 100.f;
	HuntDistance = 1500.f;

	bHasLineOfSight = false;
	bRandMoveAwayFromPlayer = false;
	TimeSinceLostLineOfSight = 0.f;
	TimeSinceRandMovement = 0.f;
	TimeSinceShotFired = 0.f;
	TimeToGetLineOfSight = 2.f;

	WeaponSlots.SetNum(1);
	SpottedPlayerPositions.SetNum(5);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACultistCharacter::BeginPlay()
{
	AEnemyCharacter::BeginPlay();
	EquipWeapon(Weapon);

	OldRotation = GetActorRotation();
	DefaultTimeToRandMove = TimeToRandMove;
	TimeToRandMove = DefaultTimeToRandMove + FMath::FRandRange(0.f, AddedRandomTime);
	TimeToBurstPause = FMath::FRandRange(MinBurstPause, MaxBurstPause);
	TimeSinceBurstPause = MinBurstPause;
	BurstSize = FMath::RandRange(MinBurstSize, MaxBurstSize);
}

void ACultistCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);

	if (bIsAlive)
	{
		if (GetCurrentState() == EEnemyState::Triggered)
		{
			GetCharacterMovement()->MaxWalkSpeed = TriggeredMoveSpeed;
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
				if (bOldHasLineOfSight && !bHasLineOfSight)
					bLostLineOfSight = true;

				if (TimeSinceLostLineOfSight < TimeToGetLineOfSight)
				{
					bHasMovedToPlayer = false;
					TimeSinceRanToLastKnownPosition = 0.f;
					TryGetLineOfSight(DeltaTime);
				}
				else
					GoToLastKnownPosition(DeltaTime);
			}
		}
		else if (GetCurrentState() == EEnemyState::Search)
		{
			GetCharacterMovement()->MaxWalkSpeed = SearchMoveSpeed;

			TimeSinceStartSearch += DeltaTime;
			if (TimeSinceStartSearch < SearchTime)
				SearchForPlayer(DeltaTime);
			else
			{
				//Debug::LogOnScreen("Done Searching, going idle");
				SetCurrentState(EEnemyState::Idle);
			}
		}
	}
}

void ACultistCharacter::CheckLineOfSight()
{
	if (PlayerReferense != NULL)
	{
		if (PlayerReferense->GetPlayerHitPoint_Chest() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Left() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Right() == NULL)
		{
			Debug::LogWarning("Hit Points on player is NULL, Skipping Check Line if Sight Function");
			return;
		}

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
			TimeSinceLostLineOfSight = 0.f;
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
		if (TimeSinceBurstPause >= TimeToBurstPause)
		{
			TimeToBurstPause = FMath::FRandRange(MinBurstPause, MaxBurstPause);

			if (TimesShot < BurstSize)
			{
				if (TimeSinceShotFired >= 1.f / (RPM / 60.f))
				{
					FVector Direction = (PlayerReferense->GetActorLocation() + FVector(FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f), FMath::FRandRange(-100, 100.f))) - GetActorLocation();
					CurrentWeapon->StartFire(Direction * 5000.f);
					CurrentWeapon->StopFire(Direction * 5000.f);
					TimesShot++;
					TimeSinceShotFired = 0.f;
				}
				TimeSinceShotFired += DeltaTime;
			}
			else
			{
				BurstSize = FMath::RandRange(MinBurstSize, MaxBurstSize);
				TimeSinceBurstPause = 0.f;
				TimeSinceShotFired = 0.f;
				TimesShot = 0.f;
			}
		}
		TimeSinceBurstPause += DeltaTime;
	}
}
void ACultistCharacter::FocusOnPosition(float DeltaTime, FVector Position)
{
	FVector Direction = Position - GetActorLocation();
	FRotator NewControlRotation = Direction.Rotation();
	NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
	CurrentControlRotation = FMath::Lerp(OldRotation, NewControlRotation, TurnRate * DeltaTime);
	FaceRotation(CurrentControlRotation);
	OldRotation = GetActorRotation();
}
void ACultistCharacter::ReactToPlayerMovement(float DeltaTime)
{
	// Notes för framtiden:
	// Kolla om fienden är åvanför spelaren och agera anorlunda utifrån detta
	float distanceToPlayer = FVector::Dist(GetActorLocation(), PlayerReferense->GetActorLocation());
	if (distanceToPlayer < RetreatDistance)
		AvoidPlayer(DeltaTime);
	//else if (distanceToPlayer > HuntDistance)
	//	Debug::LogOnScreen("TODO: Player Is far away, Should go closer");

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
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * 100.f) + (GetActorForwardVector() * 100.f));
		else if (SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Right && !SpottedPlayerPositions.Last().bCanSeePlayerShoulder_Left)
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * -100.f) + (GetActorForwardVector() * 100.f));
	}
	LastKnownPlayerPos = PlayerReferense->GetActorLocation();
}
void ACultistCharacter::AvoidPlayer(float DeltaTime)
{
	//const FName TraceTag("Debug Trace");
	FHitResult result;
	ECollisionChannel collisionChannel;
	collisionChannel = ECC_WorldDynamic;
	FCollisionQueryParams collisionQuery;
	//collisionQuery.TraceTag = TraceTag;
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	collisionQuery.bTraceComplex = true;
	FCollisionObjectQueryParams objectCollisionQuery;
	objectCollisionQuery = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	FCollisionResponseParams collisionResponse;
	collisionResponse = ECR_Block;
	collisionQuery.AddIgnoredActor(this);

	if (!bIsRandMoving)
	{
		// If Can Move Backwards
		if (!GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * -RetreatNavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorForwardVector() * -RetreatRadius);
		// Move Left If needed
		if (GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorRightVector() * -RetreatNavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * RetreatRadius);
		// Move Right If needed
		if (GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + (GetActorRightVector() * RetreatNavRadius), collisionChannel, collisionQuery, collisionResponse))
			NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + GetActorRightVector() * -RetreatRadius);
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
void ACultistCharacter::GoToLastKnownPosition(float DeltaTime)
{
	if (TimeSinceRanToLastKnownPosition > TimeToMoveTooLastKnownPosititon)
	{
		//Debug::LogOnScreen("Done Moving, Entering Search Mode");
		SetCurrentState(EEnemyState::Search);
		TimeSinceStartSearch = 0.f;
		LastFloorValue = -1.f;
	}
	else if (TimeSinceRanToLastKnownPosition == 0.f || (TimeSinceRanToLastKnownPosition > 1.5f && !bHasMovedToPlayer)) // Kommer gå till en uppdaterad position av spelaren i ett visst antal sekunder
	{
		//Debug::LogOnScreen("Moving to player position");
		if (TimeSinceRanToLastKnownPosition > 1.5f)
			bHasMovedToPlayer = true;
		NavSystem->SimpleMoveToLocation(GetController(), PlayerReferense->GetActorLocation());
		LastKnownPlayerVelocity = PlayerReferense->GetVelocity();
	}
	TimeSinceRanToLastKnownPosition += DeltaTime;
}
void ACultistCharacter::SearchForPlayer(float DeltaTime)
{
	if (LastFloorValue < FMath::FloorToInt(TimeSinceStartSearch / 3.f))
	{
		//Debug::LogOnScreen("Run Search Thingy");

		float DistToPlayer = (GetActorLocation() - PlayerReferense->GetActorLocation()).Size();

		FVector newPos;
		for (size_t i = 0; i < 10; i++)
		{
			float RandDist = FMath::FRandRange(DistToPlayer - 100.f, DistToPlayer + 100.f);
			float RandDir = FMath::FRandRange(-90.f, 90.f);
			
			newPos = GetActorLocation() + GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * RandDist;

			FPathFindingQuery pathFindingQuery;
			pathFindingQuery.bAllowPartialPaths = true;
			pathFindingQuery.StartLocation = GetActorLocation();
			pathFindingQuery.EndLocation = newPos;
			pathFindingQuery.NavData = NavSystem->GetMainNavData();
			if (NavSystem->TestPathSync(pathFindingQuery))
				break;
		}

		NavSystem->SimpleMoveToLocation(GetController(), newPos);
	}
	LastFloorValue = FMath::FloorToInt(TimeSinceStartSearch / 3.f);
}

void ACultistCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	if (GetCurrentState() != EEnemyState::Triggered)
	{
		SetCurrentState(EEnemyState::Triggered);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}
}
void ACultistCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (GetCurrentState() != EEnemyState::Triggered && Cast<APlayerCharacter>(OtherPawn))
	{
		SetCurrentState(EEnemyState::Triggered);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}
}

bool ACultistCharacter::EquipWeapon(TSubclassOf<AWeapon> Weapon)
{
	if (Weapon != NULL)
	{
		for (int32 i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i] == NULL)
			{
				const FRotator SpawnRotation = FRotator::ZeroRotator;
				const FVector SpawnLocation = FVector::ZeroVector;
				WeaponSlots[i] = GetWorld()->SpawnActor<AWeapon>(Weapon, SpawnLocation, SpawnRotation);
				WeaponSlots[i]->AttachRootComponentTo(GetMesh(), TEXT("R_WristSocket"), EAttachLocation::SnapToTargetIncludingScale, true);
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
	for (int32 i = 0; i < WeaponSlots.Num(); i++)
	{
		if (CurrentWeapon == WeaponSlots[i])
			index = i;
	}
	return index;
}

void ACultistCharacter::Death()
{
	AEnemyCharacter::Death();

	for (int32 i = 0; i < WeaponSlots.Num(); i++)
	{
		if (WeaponSlots[i] != NULL)
			WeaponSlots[i]->Destroy();

		WeaponSlots[i] = NULL;
	}
}
float ACultistCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AEnemyCharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Debug::LogOnScreen("Cultist take damage");

	if (GetCurrentState() != EEnemyState::Triggered)
	{
		SetCurrentState(EEnemyState::Triggered);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}
	//CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

	TimeSinceRandMovement += TimeToRandMove * (AvoidDamage / 100.f);
	if (TimeSinceRandMovement >= TimeToRandMove)
		bRandMoveAwayFromPlayer = true;

	//if (CurrentHealth == 0)
	//	bIsAlive = false;
	return DamageAmount;
}