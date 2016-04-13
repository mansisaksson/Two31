#include "Two31.h"
#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "AIController.h"
#include "Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bIsAlive = true;
	bAggro = false;

	RotationTimer = 2.f;

	TargetLocation = FVector::ZeroVector;

	DespawnTimer = 3.f;
	TimeSinceDeath = 0.f;
	TimeSinceRotationStart = 0.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	NavSystem = GetWorld()->GetNavigationSystem();
	AIController = Cast<AAIController>(GetController());

	if (AIController == NULL)
		UE_LOG(DebugError, Fatal, TEXT("AIController Not found!"));
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	if (!bIsAlive)
	{
		Death();
		TimeSinceDeath += DeltaTime;
		if (TimeSinceDeath > DespawnTimer)
			Destroy();
	}
	if (bAggro)
	{
		// mult is used to mask the Z-Vector during rotation.
		FVector mult = FVector(1, 1, 0);
		FVector MyLocation = GetMesh()->GetComponentLocation();
		
		FVector TargetRotation = (TargetLocation - MyLocation);
		TargetRotation *= mult;
		TargetRotation.Normalize();

		SetActorRotation(FMath::Lerp(GetActorRotation(), TargetRotation.Rotation(), 0.05f));
		
		TimeSinceRotationStart += DeltaTime;
		if (TimeSinceRotationStart > RotationTimer)
		{
			bAggro = false;
			TimeSinceRotationStart = 0;
		}


	}
}

void AEnemyCharacter::Take_Damage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, 100.f);
	if (CurrentHealth == 0)
		bIsAlive = false;
}

void AEnemyCharacter::OnHearNoise(APawn *OtherPawn, const FVector &Location, float Volume)
{
	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + OtherPawn->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

void AEnemyCharacter::OnSeePawn(APawn *OtherPawn)
{
	FString message = TEXT("Saw Pawn ") + OtherPawn->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DamageTaken"));
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, 100.f);
	if (CurrentHealth == 0)
		bIsAlive = false;

	TargetLocation = DamageCauser->GetActorLocation();
	if(!bAggro)
		bAggro = true;

	return DamageAmount;
}

float AEnemyCharacter::GetHealth()
{
	return CurrentHealth;
}

void AEnemyCharacter::Death()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}