#include "Two31.h"
#include "ImpCharacter.h"
#include "PlayerCharacter.h"

AImpCharacter::AImpCharacter()
	: AEnemyCharacter()
{
	AttackDamage = 10.f;
	AttackCooldown = 3.f;
	TimeSinceLastAttack = 0.f;
	TimeSinceRotationStart = 0.f;
	TimeSinceMoveUpdate = 0.f;
	TimeToMoveUpdate = 0.5f;

	DistanceToPlayer = 100000.f;
	RangeToAttack = 500.f;
	MinDistanceRandomSearch = 0.f;
	DistOffsetInSearch = 500.f;

	bRandomSearch = true;
	bMoveToLastKnown = true;

	RotationTimer = 0.3f;

	LastKnowPosition = FVector::ZeroVector;
	TimeSinceLostLineOfSight = 0.f;
	TimeToIdle = 5.f;

	ExtraTimeToGetLocation = 1.f;
	ExtraTimeToMove = 0.f;

	bAttackOnCooldown = false;

	AttackRadius = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRadius"));
	AttackRadius->AttachTo(GetMesh());
	AttackRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AImpCharacter::PostInitializeComponents()
{
	AEnemyCharacter::PostInitializeComponents();

	AttackRadius->OnComponentBeginOverlap.AddDynamic(this, &AImpCharacter::OnAttackBeginOverlap);
}

void AImpCharacter::BeginPlay()
{
	AEnemyCharacter::BeginPlay();


}

void AImpCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);

	if (bIsAlive)
	{
		if (GetCurrentState() == EEnemyState::Triggered)
		{
			//Debug::LogOnScreen("triggered state");
			// Can you see the player?
			if (CanSeePlayer())
			{
				// Reset values for extra search functions that are used when sight of the player has been lost.
				bRandomSearch = true;
				bMoveToLastKnown = true;
				ExtraTimeToMove = 0.f;

				// Check whether the imp is able to attack the player or not.
				DistanceToPlayer = GetDistanceToPlayer();
				if (DistanceToPlayer < RangeToAttack)
				{
					// check attack cool-down and such
					// Attack()
					//Debug::LogOnScreen("Can Attack");
				}
				else
				{
					// Update the imps destination once the TimeToMoveUpdate has been reached.
					TimeSinceMoveUpdate += DeltaTime;
					if (TimeSinceMoveUpdate > TimeToMoveUpdate/* 0.5f*/)
					{
						NavSystem->SimpleMoveToLocation(GetController(), PlayerReferense->GetActorLocation());
						TimeSinceMoveUpdate = 0.f;
					}
				}
			}
			else
			{
				// Allow the imp to obtain the players position for an extra X amount of time. ( ExtraTimeToGetLocation )
				ExtraTimeToMove += DeltaTime;
				if (ExtraTimeToMove < ExtraTimeToGetLocation)
					LastKnowPosition = PlayerReferense->GetActorLocation();

				// Check if the imp as the last known position (LKP), if not move it there
				// once the imp has arrived at the LKP it should not longer attempt to move to the LKP and instead search for the player
				if (AtLastKnownPosition() && bMoveToLastKnown)
					bMoveToLastKnown = false;
				else if (!AtLastKnownPosition() && bMoveToLastKnown)
					NavSystem->SimpleMoveToLocation(GetController(), LastKnowPosition);
				else
				{
					// Perform one random search based on the players LKP,
					// if successful in locating the player, functions above will reset values 
					// otherwise wait timeout to idle.
					TimeSinceLostLineOfSight += DeltaTime;
					if (TimeSinceLostLineOfSight < TimeToIdle && bRandomSearch)
					{
						MoveToPlayersEstimatedPosition();
						bRandomSearch = false;
					}
					else if (TimeSinceLostLineOfSight > TimeToIdle)
					{
						SetCurrentState(EEnemyState::Idle);
						//Debug::LogOnScreen("Going Idle");
						TimeSinceLostLineOfSight = 0.f;
						bRandomSearch = true;
					}
				}
			}
		}
		else if (GetCurrentState() == EEnemyState::Search)
		{
			// Transition state from idle to triggered, the transition will end when the imp has rotated to face the player.
			//Debug::LogOnScreen("search state");
			RotateTowardsPlayer();
			TimeSinceRotationStart += DeltaTime;
			if (TimeSinceRotationStart > RotationTimer)
			{
				SetCurrentState(EEnemyState::Triggered);
				LastKnowPosition = PlayerReferense->GetActorLocation();
				TimeSinceRotationStart = 0.f;
			}
		}
		else if (GetCurrentState() == EEnemyState::Idle)
		{
			// Go idle
			//Debug::LogOnScreen("Idle");
		}
	}
}

void AImpCharacter::NotifyActorBeginOverlap(AActor* actor)
{
	Super::NotifyActorBeginOverlap(actor);
	//Debug::LogOnScreen("Test");
	if (Cast<ANavLinkProxy>(actor))
	{
		ANavLinkProxy* NavLink = Cast<ANavLinkProxy>(actor);
		Debug::LogOnScreen("Nav link proxy");
		SetTimeToMoveUpdate(10.f);
	}
}

bool AImpCharacter::CanSeePlayer()
{
	if (PlayerReferense != NULL)
	{
		if (PlayerReferense->GetPlayerHitPoint_Chest() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Left() == NULL || PlayerReferense->GetPlayerHitPoint_Shoulder_Right() == NULL)
			return false;//Debug::LogFatalError("Hit Points on player is NULL");

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


		SeenPositions seenPositions;
		for (size_t i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Chest()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
				if (Cast<APlayerCharacter>(result.GetActor()))
					seenPositions.bCanSeePlayerChest = true;
			}
			else if (i == 1)
			{
				GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Left()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
				if (Cast<APlayerCharacter>(result.GetActor()))
					seenPositions.bCanSeePlayerShoulder_Left = true;
			}
			else if (i == 2)
			{
				GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), PlayerReferense->GetPlayerHitPoint_Shoulder_Right()->GetComponentLocation(), collisionChannel, collisionQuery, collisionResponse);
				if (Cast<APlayerCharacter>(result.GetActor()))
					seenPositions.bCanSeePlayerShoulder_Right = true;
			}
		}

		bOldLineOfSight = bLineOfSight;
		if (seenPositions.bCanSeePlayerChest || seenPositions.bCanSeePlayerShoulder_Left || seenPositions.bCanSeePlayerShoulder_Right)
		{
			bLineOfSight = true;
			TimeSinceLostLineOfSight = true;
			LastKnowPosition = PlayerReferense->GetActorLocation();
		}
		else
			bLineOfSight = false;
	}

	return bLineOfSight;
}

float AImpCharacter::GetDistanceToPlayer()
{
	FVector distance = PlayerReferense->GetActorLocation() - GetActorLocation();
	return FMath::Abs(distance.Size()); 
}

bool AImpCharacter::AtLastKnownPosition()
{
	if (GetActorLocation().X > LastKnowPosition.X - 50 && GetActorLocation().X < LastKnowPosition.X + 50 && GetActorLocation().Y > LastKnowPosition.Y - 50 && GetActorLocation().Y < LastKnowPosition.Y + 50)
		return true;
	return false;
}

void AImpCharacter::SetTimeToMoveUpdate(float Time)
{
	TimeToMoveUpdate = Time;
}

void AImpCharacter::RotateTowardsPlayer()
{
	//Debug::LogOnScreen("rotating");
	// Mask to not rotate in Z-Vector
	FVector Mask = FVector(1, 1, 0);
	FVector TargetRotation = (PlayerReferense->GetActorLocation() - GetActorLocation());
	TargetRotation *= Mask;
	TargetRotation.Normalize();

	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetRotation.Rotation(), 0.05f));
}

void AImpCharacter::MoveToPlayersEstimatedPosition()
{
	//Debug::LogOnScreen("Rotating towards random position");
	float DistToPlayer = GetDistanceToPlayer() + DistOffsetInSearch;

	float RandDist = FMath::FRandRange(MinDistanceRandomSearch, DistToPlayer);
	float RandDir = FMath::FRandRange(-90.f, 90.f);
	FVector newPos = GetActorLocation();
	newPos += GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * RandDist;

	NavSystem->SimpleMoveToLocation(GetController(), newPos);

	//Debug::LogOnScreen(FString::Printf(TEXT("Rand pos  is '%f'  '%f'" ), newPos.X , newPos.Y));
}

void AImpCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	if (GetCurrentState() == EEnemyState::Idle)
	{
		Debug::LogOnScreen("Aggro true - hear pawn");
		SetCurrentState(EEnemyState::Search);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}
}

void AImpCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (GetCurrentState() == EEnemyState::Idle)
	{
		Debug::LogOnScreen("Aggro true - see pawn");
		SetCurrentState(EEnemyState::Search);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}
}

float AImpCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AEnemyCharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (GetCurrentState() != EEnemyState::Triggered)
	{
		Debug::LogOnScreen("Aggro true - take DAMAGE");
		SetCurrentState(EEnemyState::Search);
		GetOverlappingActors(AlertRadius, AEnemyCharacter::GetClass());
	}

	return DamageAmount;
}

void AImpCharacter::OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (!bAttackOnCooldown)
		{
			Debug::LogOnScreen("Enemy attacking Player");
			APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			Player->TakeDamage(AttackDamage, DamageEvent, PlayerController, this);
			bAttackOnCooldown = true;
		}
	}
}

void AImpCharacter::Death()
{
	AEnemyCharacter::Death();
	AttackRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}