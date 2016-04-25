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
	TimeToMoveUpdate = 0.1f;

	DistanceToPlayer = 100000.f;
	RangeToAttack = 500.f;
	DistOffsetInSearch = 500.f;

	bAggro = false;
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

	AlertRadius = CreateDefaultSubobject<USphereComponent>(TEXT("AlertRadius"));
	AlertRadius->AttachTo(GetMesh());
	AlertRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
			// Can you see the player?
			if (CanSeePlayer())
			{
				//Debug::LogOnScreen("Can see the player");
				bRandomSearch = true;
				bMoveToLastKnown = true;
				DistanceToPlayer = GetDistanceToPlayer();
				//Debug::LogOnScreen(FString::Printf(TEXT("Distance is '%f'" ),DistanceToPlayer));
				//Debug::LogOnScreen(FString::Printf(TEXT("Range to attack is '%f'"), RangeToAttack));

				// Are you in range to attack ?
				if (DistanceToPlayer < RangeToAttack)
				{
					// check attack cool-down and such
					// Attack()
					//Debug::LogOnScreen("Can Attack");
				}
				else
				{
					//NavSystem->SimpleMoveToLocation(GetController(), LastKnowPosition);

					TimeSinceMoveUpdate += DeltaTime;
					if (TimeSinceMoveUpdate > TimeToMoveUpdate/* 0.5f*/)
					{
						//NavSystem->SimpleMoveToActor(GetController(), PlayerReferense);
						NavSystem->SimpleMoveToLocation(GetController(), PlayerReferense->GetActorLocation());
						TimeSinceMoveUpdate = 0.f;
					}

				}
				ExtraTimeToMove = 0.f;
			}
			else
			{
				ExtraTimeToMove += DeltaTime;
				if (ExtraTimeToMove < ExtraTimeToGetLocation)
					LastKnowPosition = PlayerReferense->GetActorLocation();
				if (AtLastKnownPosition() && bMoveToLastKnown)
				{
					Debug::LogOnScreen("At last known position");
					bMoveToLastKnown = false;
				}
				else if (!AtLastKnownPosition() && bMoveToLastKnown)
				{
					//Debug::LogOnScreen(FString::Printf(TEXT("Last known position '%f' , '%f'"), LastKnowPosition.X, LastKnowPosition.Y));
					//Debug::LogOnScreen(FString::Printf(TEXT("Current position '%f' , '%f'"), GetActorLocation().X, GetActorLocation().Y));
					NavSystem->SimpleMoveToLocation(GetController(), LastKnowPosition);
				}
				else
				{
					// time since lost line of sight.

					// move to players last known position, done above
					// get players last known rotation & location and estimate his whereabouts
					// move to where it is believed the player might be
					// located player
						// no - go idle
						// yes - continue above
					
					TimeSinceLostLineOfSight += DeltaTime;
					if (TimeSinceLostLineOfSight < TimeToIdle && bRandomSearch)
					{
						MoveToPlayersEstimatedPosition();
						bRandomSearch = false;
					}
					else if (TimeSinceLostLineOfSight > TimeToIdle)
					{
						SetCurrentState(EEnemyState::Idle);
						Debug::LogOnScreen("Going Idle");
						TimeSinceLostLineOfSight = 0.f;
						bRandomSearch = true;
					}

					//MoveToPlayersEstimatedPosition();

				}
			}
		}
		else if (GetCurrentState() == EEnemyState::Search)
		{
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
			if (bAggro)
			{
				SetCurrentState(EEnemyState::Search);
				GetOverlappingActors(AlertRadius, AImpCharacter::GetClass());
				//GetOverlappingActors(AlertRadius, ANavLinkProxy::GetClass());
				//Debug::LogOnScreen("Getting overlapping actors");
			}

		}
			// can you see the player
				// yes 
					// is the player within attack range
						// yes 
							// is attack on cool-down
								// no - attack 
								// yes - 
						// no - run to player 
				// no 
					// are you at the last seen position
						// no - move there
						// yes - start idle timer
							// has idle timer been reached
								// go search
		// State Search
			// look for player
				// rotation ? 
				// move ? 
					// failure to locate player - go idle
		// State Idle
			// if you take damage 
				// rotate to face player
				// alert nearby imps to also face player 	
	}
}

void AImpCharacter::NotifyActorBeginOverlap(AActor* actor)
{
	Super::NotifyActorBeginOverlap(actor);
	//Debug::LogOnScreen("Test");
	if (Cast<ANavLinkProxy>(actor))
	{
		ANavLinkProxy* NavLink = Cast<ANavLinkProxy>(actor);
		Debug::LogOnScreen("navlinkproxy");
		SetTimeToMoveUpdate(10.f);
	}
	if (Cast<UBoxComponent>(actor))
	{
		Debug::LogOnScreen("Actor overlap");
		AActor* thing = Cast<UBoxComponent>(actor);
		if (thing->ActorHasTag("Jump"))
			Debug::LogOnScreen("The chosen one has been located");
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
	// Mask to not rotate in Z-Vector
	FVector Mask = FVector(1, 1, 0);
	FVector TargetRotation = (PlayerReferense->GetActorLocation() - GetActorLocation());
	TargetRotation *= Mask;
	TargetRotation.Normalize();

	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetRotation.Rotation(), 0.05f));
}

void AImpCharacter::MoveToPlayersEstimatedPosition()
{
	Debug::LogOnScreen("Rotating towards random position");
	float DistToPlayer = GetDistanceToPlayer() + DistOffsetInSearch;


	float RandDist = FMath::FRandRange(0.f, DistToPlayer);
	float RandDir = FMath::FRandRange(-90.f, 90.f);
	FVector newPos = GetActorLocation();
	newPos += GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * RandDist;

	NavSystem->SimpleMoveToLocation(GetController(), newPos);

	Debug::LogOnScreen(FString::Printf(TEXT("Rand pos  is '%f'  '%f'" ), newPos.X , newPos.Y));
}

void AImpCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	//AEnemyCharacter::OnHearNoise(OtherActor, Location, Volume);
	//const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	//FString message = TEXT("Heard Actor ") + OtherActor->GetName() + VolumeDesc;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	//NavSystem->SimpleMoveToLocation(GetController(), OtherActor->GetActorLocation());
	// TODO: game-specific logic
}

void AImpCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (GetCurrentState() == EEnemyState::Idle)
	{
		bAggro = true;
		Debug::LogOnScreen("Aggro true - see pawn");
	}
	//NavSystem->SimpleMoveToActor(GetController(), PlayerReferense);

}

void AImpCharacter::SetCurrentState(EEnemyState State)
{
	AEnemyCharacter::SetCurrentState(State);
}

float AImpCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AEnemyCharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (GetCurrentState() != EEnemyState::Triggered)
	{
		Debug::LogOnScreen("Aggro true - take damage");
		bAggro = true;
	}

	//GetOverlappingActors(AlertRadius, AImpCharacter::GetClass());

	return DamageAmount;
}

void AImpCharacter::OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy inrange of something"));
	if (Cast<APlayerCharacter>(OtherActor))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (!bAttackOnCooldown)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy attacking Player"));
			APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			Player->TakeDamage(AttackDamage, DamageEvent, PlayerController, this);
			bAttackOnCooldown = true;
		}
	}
}

void AImpCharacter::GetOverlappingActors(UShapeComponent* Sphere, UClass* ClassFilter)
{
	TArray<AActor*> OverlappingEnemies;
	Sphere->GetOverlappingActors(OverlappingEnemies, ClassFilter);
	for (size_t i = 0; i < OverlappingEnemies.Num(); i++)
	{
		if (Cast<AImpCharacter>(OverlappingEnemies[i]))
		{
			AImpCharacter* imp = Cast<AImpCharacter>(OverlappingEnemies[i]);
			imp->bAggro = false;
			imp->SetCurrentState(EEnemyState::Search);
		}
		if (Cast<ANavLinkProxy>(OverlappingEnemies[i]))
		{
			Debug::LogOnScreen("Overlapping navlinkprox");
		}
	}
	Debug::LogOnScreen("Triggering other imps");
}

void AImpCharacter::Death()
{
	AEnemyCharacter::Death();
	AttackRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}