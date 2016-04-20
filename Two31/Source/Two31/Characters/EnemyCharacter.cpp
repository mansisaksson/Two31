#include "Two31.h"
#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "../MusicManager.h"
#include "../Characters/PlayerCharacter.h"
#include "AIController.h"
#include "Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	MaxHealth = 100.f;
	bIsAlive = true;

	DespawnTimer = 3.f;
	TimeSinceDeath = 0.f;

	EnemyState = EEnemyState::Idle;


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
	CurrentHealth = MaxHealth;

	NavSystem = GetWorld()->GetNavigationSystem();
	AIController = Cast<AAIController>(GetController());

	if (AIController == NULL)
		UE_LOG(DebugError, Warning, TEXT("AIController Not found!"));
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
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	if (CurrentHealth == 0)
		bIsAlive = false;
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
	MusicManager::RemoveEnemy(EnemyState);
}

void AEnemyCharacter::SetCurrentState(EEnemyState State)
{
	MusicManager::OnEnemyStateSwitch(EnemyState, State);
	EnemyState = State;
}