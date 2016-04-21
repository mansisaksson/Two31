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

	TriggeredMoveSpeed = 500.f;
	SearchMoveSpeed = 200.f;
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
	RetreatRadius = 80.f;
	RetreatNavRadius = 100.f;
	HuntDistance = 1500.f;

	bHasLineOfSight = false;
	bRandMoveAwayFromPlayer = false;
	TimeSinceLostLineOfSight = 0.f;
	TimeSinceRandMovement = 0.f;
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
					TimeSinceRanToLastKnownPosition = 0.f;
					TryGetLineOfSight(DeltaTime);
				}
				else
					GoToLastKnownPosition(DeltaTime);
			}
		}
		else if (GetCurrentState() == EEnemyState::Search)
		{
			GetCharacterMovement()->MaxWalkSpeed = TriggeredMoveSpeed;
			SearchForPlayer(DeltaTime);
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
	FaceRotation(FMath::Lerp(OldRotation, NewControlRotation, TurnRate * DeltaTime));
	//FaceRotation(NewControlRotation);
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
		Debug::LogOnScreen("Done Moving, Entering Search Mode");
		SetCurrentState(EEnemyState::Search);
	}
	else if (TimeSinceRanToLastKnownPosition < 1.5f) // Kommer gå till en uppdaterad position av spelaren i ett visst antal sekunder
	{
		Debug::LogOnScreen("Moving to player position");
		NavSystem->SimpleMoveToLocation(GetController(), PlayerReferense->GetActorLocation());
	}
	TimeSinceRanToLastKnownPosition += DeltaTime;
}
void ACultistCharacter::SearchForPlayer(float DeltaTime)
{
	// Gå till en random position runt sig
	// FaceLocation(framåt)

	// if (x time has passed)
	//		move too location(spawn location)
	//		go idle;

	/*FNavLocation NewPos;
	NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), MaxRandMoveRadius, NewPos);
	NavSystem->SimpleMoveToLocation(GetController(), NewPos.Location);*/
}

void ACultistCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	SetCurrentState(EEnemyState::Triggered);
}
void ACultistCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (Cast<APlayerCharacter>(OtherPawn))
		SetCurrentState(EEnemyState::Triggered);
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
	//Debug::LogOnScreen(FString::Printf(TEXT("Current Health: %f"), CurrentHealth));

	TimeSinceRandMovement += TimeToRandMove * (AvoidDamage / 100.f);
	if (TimeSinceRandMovement >= TimeToRandMove)
		bRandMoveAwayFromPlayer = true;

	if (CurrentHealth == 0)
		bIsAlive = false;
	return DamageAmount;
}