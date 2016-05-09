#include "Two31.h"
#include "ImpCharacter.h"
#include "Engine.h"
#include "PlayerCharacter.h"

AImpCharacter::AImpCharacter()
	: AEnemyCharacter()
{
	AttackDamage = 10.f;
	AttackMoveCooldown = 0.5f;
	MinAttackRange = 140.f;
	MaxAttackRange = 200.f;
	TimeSinceLastAttack = 0.f;
	TimeSinceRotationStart = 0.f;
	TimeSinceMoveUpdate = 0.f;
	TimeToMoveUpdate = 0.5f;
	MoveAroundTimer = 0.f;
	LandCooldown = 0.5f;

	MinFlankDegree = 50.f;
	MaxFlankDegree = 90.f;
	MoveAroundLocationMin = -5.f;
	MoveAroundLocationMax = 5.f;
	MinDistanceRandomSearch = 0.f;
	DistOffsetInSearch = 500.f;
	SideStepDistance = 200.f;

	MaxDegreeLocatedRight = 70.f;
	MinDegreeLocatedLeft = 110.f;

	AllowedDistanceFromPlayer = 2000.f;
	AllowedDistanceFromPlayerOffset = 0.f;

	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	HalfWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed / 2);

	bMoveCloser = true;
	bMoveAroundPlayer = true;
	bMoveOnce = true;
	bRandomSearch = true;
	bMoveToLastKnown = true;
	bRepositioned = true;
	bPlayAttackAnimation = false;

	bForceMovement = false;
	ForcedMovementDirection = FVector::ZeroVector;
	MoveAroundLocation = FVector::ZeroVector;

	RotationTimer = 0.4f;

	PlayerPositionedWhenAggro = FVector::ZeroVector;
	PlayerForwardVectorWhenAggro = FVector::ZeroVector;
	LastKnowPosition = FVector::ZeroVector;
	TimeSinceLostLineOfSight = 0.f;
	TimeToIdle = 5.f;

	ExtraTimeToGetLocation = 1.f;
	ExtraTimeToMove = 0.f;

	bAttackOnCooldown = false;

	L_ClawRadius = CreateDefaultSubobject<USphereComponent>(TEXT("L_ClawRadius"));
	L_ClawRadius->AttachTo(GetMesh(), TEXT("L_Claw"));
	L_ClawRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	R_ClawRadius = CreateDefaultSubobject<USphereComponent>(TEXT("R_ClawRadius"));
	R_ClawRadius->AttachTo(GetMesh(), TEXT("R_Claw"));
	R_ClawRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AImpCharacter::PostInitializeComponents()
{
	AEnemyCharacter::PostInitializeComponents();

	L_ClawRadius->OnComponentBeginOverlap.AddDynamic(this, &AImpCharacter::OnAttackBeginOverlap);
	R_ClawRadius->OnComponentBeginOverlap.AddDynamic(this, &AImpCharacter::OnAttackBeginOverlap);
}

void AImpCharacter::BeginPlay()
{
	AEnemyCharacter::BeginPlay();
	OldRotation = GetActorRotation();

	DefaultClawRadius = L_ClawRadius->GetUnscaledSphereRadius();
	TimeSinceLand = LandCooldown;
	SetClawRadius(0.f, 0.f);
}

void AImpCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);

	if (bIsAlive)
	{
		TimeSinceLand += DeltaTime;
		if (GetCharacterMovement()->IsFalling())
			TimeSinceLand = 0.f;
		
		if (TimeSinceLand >= LandCooldown)
		{
			if (bForceMovement)
			{
				GetController()->StopMovement();
				AddMovementInput(ForcedMovementDirection, 1.0f, true);
			}
			else if (GetCurrentState() == EEnemyState::Triggered)
			{
				// Check if the imp should flank around the player - should only be capable of performing this task once.
				if (bMoveAroundPlayer)
				{
					if (bMoveOnce)
						MoveAroundPlayer();
					else if (GetActorLocation().X > MoveAroundLocation.X - 50 && GetActorLocation().X < MoveAroundLocation.X + 50 && GetActorLocation().Y > MoveAroundLocation.Y - 50 && GetActorLocation().Y < MoveAroundLocation.Y + 50)
					{
						Debug::LogOnScreen("At location - moving towards");
						bMoveAroundPlayer = false;
					}
					else if (GetDistanceToPlayer() < 1000.f)
					{
						Debug::LogOnScreen("Close to player - moving towards");
						bMoveAroundPlayer = false;
					}
					else if (GetDistanceToPlayer() > AllowedDistanceFromPlayer)
						bMoveAroundPlayer = false;

					// Failsafe if it takes too long to move to player
					MoveAroundTimer += DeltaTime;
					if (MoveAroundTimer > 5.f)
						bMoveAroundPlayer = false;
				}
				else if (CanSeePlayer())
				{
					// Reset values for extra search functions that are used when sight of the player has been lost.
					bRandomSearch = true;
					bMoveToLastKnown = true;
					ExtraTimeToMove = 0.f;

					// Check distance to player
					// if in range to attack then bplayAttackanimation = true
					// else in range to attack = false && navsystem->simplemovetoActor
					if (GetDistanceToPlayer() > MinAttackRange && bMoveCloser)
					{
						bPlayAttackAnimation = false;

						TimeSinceLastAttack += DeltaTime;
						if (TimeSinceLastAttack >= AttackMoveCooldown)
							NavSystem->SimpleMoveToActor(GetController(), PlayerReferense);
						else
							FocusOnPosition(PlayerReferense->GetActorLocation());
					}
					else
					{
						if (GetDistanceToPlayer() > MaxAttackRange)
							bMoveCloser = true;
						else
							bMoveCloser = false;

						FocusOnPosition(PlayerReferense->GetActorLocation());

						if (!bPlayAttackAnimation)
							NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation());

						bPlayAttackAnimation = true;
						TimeSinceLastAttack = 0.f;
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
							TimeSinceLostLineOfSight = 0.f;
							bRandomSearch = true;
						}
					}
				}
			}
			else if (GetCurrentState() == EEnemyState::Search)
			{
				// Transition state from idle to triggered, the transition will end when the imp has rotated to face the player.
				RotateTowardsPlayer();
				TimeSinceRotationStart += DeltaTime;
				if (TimeSinceRotationStart > RotationTimer)
				{
					TimeSinceLastAttack = AttackMoveCooldown;
					SetCurrentState(EEnemyState::Triggered);
					PlayerPositionedWhenAggro = PlayerReferense->GetActorLocation();
					PlayerForwardVectorWhenAggro = PlayerReferense->GetActorForwardVector();
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

void AImpCharacter::ForceMovement(FVector Direction)
{
	bForceMovement = true;
	ForcedMovementDirection = Direction;
}
void AImpCharacter::StopForcedMovement()
{
	bForceMovement = false;
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
	// Move to a random location based on a 180 degree angle from the imps POV
	float DistToPlayer = GetDistanceToPlayer() + DistOffsetInSearch;
	FVector newPos;

	for (size_t i = 0; i < 10; i++)
	{
		float RandDist = FMath::FRandRange(MinDistanceRandomSearch, DistToPlayer);
		float RandDir = FMath::FRandRange(-90.f, 90.f);

		newPos = GetActorLocation();
		newPos += GetActorForwardVector().RotateAngleAxis(RandDir, FVector(0, 0, 1)) * RandDist;

		if (PathFidningQuery(newPos))
			break;
	}

	NavSystem->SimpleMoveToLocation(GetController(), newPos);
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
	GetPositionOfImps();

	return DamageAmount;
}

void AImpCharacter::MoveAroundPlayer()
{
	TArray<AActor*> OverlappingImps;
	AlertRadius->GetOverlappingActors(OverlappingImps, AImpCharacter::GetClass());
	if (!(OverlappingImps.Num() == 0))
	{
		FVector movePosition;
		FVector temp;
		float Dist = GetDistanceToPlayer() - FMath::FRandRange(0, (GetDistanceToPlayer()/2));
		AllowedDistanceFromPlayer = GetDistanceToPlayer() + AllowedDistanceFromPlayerOffset;
		//float Dist = GetDistanceToPlayer() + 5000.f;

		for (int i = 0; i < 10; i++)
		{
			float RandRotation = FMath::FRandRange(MoveAroundLocationMin, MoveAroundLocationMax);
			movePosition = PlayerPositionedWhenAggro;
			temp = movePosition + PlayerReferense->GetActorForwardVector().RotateAngleAxis(RandRotation, FVector(0, 0, 1)) * Dist;

			if (WallInWay(temp))
			{
				temp = PlayerReferense->GetActorLocation();
				bMoveAroundPlayer = false;
			}

			if (PathFidningQuery(temp))
				break;
		}
		NavSystem->SimpleMoveToLocation(GetController(), temp);
		MoveAroundLocation = temp;
	}
	else
		bMoveAroundPlayer = false;

	bMoveOnce = false;
}
void AImpCharacter::Reposition()
{
	//NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorForwardVector() * -200) + (GetActorRightVector() * 200));

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

	int32 Random = FMath::RandRange(0, 1);

	GetCharacterMovement()->MaxWalkSpeed = HalfWalkSpeed;

	//if( Random == 0 )
	//	NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * SideStepDistance));
	//else if (Random == 1)
	//	NavSystem->SimpleMoveToLocation(GetController(), GetActorLocation() + (GetActorRightVector() * -SideStepDistance));


	bRepositioned = false;
}
void AImpCharacter::FocusOnPosition(FVector Location)
{
	FVector Direction = Location - GetActorLocation();
	FRotator NewControlRotation = Direction.Rotation();
	SetActorRotation(NewControlRotation);
}

bool AImpCharacter::WallInWay(FVector Position)
{
	const FName TraceTag("Debug Trace");
	TArray<FHitResult> results;
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

	bool hitObject = GetWorld()->LineTraceMultiByChannel(results, GetActorLocation(), Position, collisionChannel, collisionQuery, collisionResponse);
	if (hitObject)
	{
		for (size_t i = 0; i < results.Num(); i++)
		{
			if (results[i].GetActor() != NULL)
			{
				if (results[i].GetComponent() != NULL && results[i].GetComponent()->Mobility == EComponentMobility::Static)
					return true;
			}
		}
	}
	return false;
}

void AImpCharacter::GetPositionOfImps()
{
	// Function to determine how many nearby imps there are and there location to the current imp
	// imps will attempt to flank the player based on the current imps position, imps to the right are given a random position further right and left vice versa
	// imps that are either in-front or behind the current imp will either run slightly to the left or right then towards the player.
	TArray<AActor*> OverlappingImps;
	AlertRadius->GetOverlappingActors(OverlappingImps, AImpCharacter::GetClass());

	for (size_t i = 0; i < OverlappingImps.Num(); i++)
	{
		if (Cast<AImpCharacter>(OverlappingImps[i]))
		{
			AImpCharacter* imp = Cast<AImpCharacter>(OverlappingImps[i]);

			// This function determines the location of the second imp from the first by comparing the right vector of the second imp to the direction vector from the current to the other.
			// The degree in the angle formed determines the other imps location to the current.
			// 0 degrees = exactly right of, while 180 degrees = exactly left. In-front or behind are both 90 degrees.
			// Right vector must be used instead of forward vector in order to differentiate between left and right.

			FVector OtherImpRight = OverlappingImps[i]->GetActorRightVector();
			FVector OtherImpLocation = OverlappingImps[i]->GetActorLocation();
			FVector ThisImpLocation = GetActorLocation();
			FVector Direction = OtherImpLocation - ThisImpLocation;

			float DotProd = FVector::DotProduct(OtherImpRight, Direction);
			float Length = OtherImpRight.Size() * Direction.Size();

			float radians = DotProd / Length;

			float degree = FMath::Acos(radians);
			degree = FMath::RadiansToDegrees(degree);
			if (degree < MaxDegreeLocatedRight)
				imp->SetRunAroundDegree(MinFlankDegree, MaxFlankDegree);
			else if (degree > MaxDegreeLocatedRight && degree < MinDegreeLocatedLeft)
				imp->SetRunAroundDegree(-10.f, 10.f);
			else if (degree > MinDegreeLocatedLeft)
				imp->SetRunAroundDegree(-MaxFlankDegree, -MinFlankDegree);

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Degrees is %f"), degree));
		}	
	}
}
void AImpCharacter::SetRunAroundDegree(float Min, float Max)
{
	MoveAroundLocationMin = Min;
	MoveAroundLocationMax = Max;
}

bool AImpCharacter::PathFidningQuery(FVector Position)
{
	FPathFindingQuery pathFindingQuery;
	pathFindingQuery.bAllowPartialPaths = true;
	pathFindingQuery.StartLocation = GetActorLocation();
	pathFindingQuery.EndLocation = Position;
	pathFindingQuery.NavData = NavSystem->GetMainNavData();
	if (NavSystem->TestPathSync(pathFindingQuery))
		return true;
	return false;
}

void AImpCharacter::Attack()
{

}

void AImpCharacter::GetOverlappingActors(UShapeComponent* Sphere, UClass* ClassFilter)
{
	AEnemyCharacter::GetOverlappingActors(Sphere, ClassFilter);
	GetPositionOfImps();
}
void AImpCharacter::OnAttackBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Debug::LogOnScreen("Imp attacking Player");
		APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		Player->TakeDamage(AttackDamage, DamageEvent, PlayerController, this);
	}
}

void AImpCharacter::Death()
{
	AEnemyCharacter::Death();
	SetClawRadius(0.f, 0.f);
}

// Denna funktion körs via animations-blueprinten för att synka upp den med attak-animationerna
void AImpCharacter::SetClawRadius(float LeftRadius, float RightRadius)
{ 
	if (LeftRadius == 0.f)
		L_ClawRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		L_ClawRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (RightRadius == 0.f)
		R_ClawRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		R_ClawRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	L_ClawRadius->SetSphereRadius(LeftRadius);
	R_ClawRadius->SetSphereRadius(RightRadius); 
}