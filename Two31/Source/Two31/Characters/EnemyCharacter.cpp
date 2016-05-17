#include "Two31.h"
#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "../MusicManager.h"
#include "../Characters/PlayerCharacter.h"
#include "../StatsPornManager.h"
#include "../BloodParticleBall.h"
#include "ImpCharacter.h"
#include "AIController.h"
#include "Engine.h"
#include "../DefaultGameMode.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	MaxHealth = 100.f;
	bIsAlive = true;
	bFirstTick = true;

	DespawnTimer = 3.f;
	TimeSinceDeath = 0.f;

	EnemyState = EEnemyState::Idle;
	UMusicManager::AddEnemy(EnemyState);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));

	AlertRadius = CreateDefaultSubobject<USphereComponent>(TEXT("AlertRadius"));
	AlertRadius->AttachTo(GetMesh());
	AlertRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AlertRadius->IgnoreActorWhenMoving(this, true);
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

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Cast<APlayerCharacter>(*Itr))
			PlayerReferense = Cast<APlayerCharacter>(*Itr);
	}
	if (PlayerReferense == NULL)
		Debug::LogFatalError("Could not find Player Character!");

	NavSystem = GetWorld()->GetNavigationSystem();
	AIController = Cast<AAIController>(GetController());

	if (AIController == NULL)
		Debug::LogFatalError("AIController Not found!");

	DefaultGameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode());
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	if (bFirstTick)
	{
		UStatsPornManager::IncreaseAmountOfEnemies();
		bFirstTick = false;
	}
	if (!bIsAlive)
	{
		TimeSinceDeath += DeltaTime;
		if (TimeSinceDeath > DespawnTimer)
			Destroy();

		for (size_t i = 0; i < DelayedImpulses.Num(); i++) {
			Debug::LogOnScreen(FString::Printf(TEXT("Add Impulse! | Strength: %f"), DelayedImpulses[i].Impulse.Size()));
			GetMesh()->AddImpulseAtLocation(DelayedImpulses[i].Impulse, DelayedImpulses[i].Location);
		}
		//DelayedImpulses.Empty();
	}
	
	DelayedImpulses.Empty();
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

void AEnemyCharacter::GetOverlappingActors(UShapeComponent* Sphere, UClass* ClassFilter)
{
	TArray<AActor*> OverlappingEnemies;
	Sphere->GetOverlappingActors(OverlappingEnemies, ClassFilter);
	for (size_t i = 0; i < OverlappingEnemies.Num(); i++)
	{
		if (Cast<AImpCharacter>(OverlappingEnemies[i]))
		{
			AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OverlappingEnemies[i]);
			enemy->SetCurrentState(EEnemyState::Search);
		}
		else if (Cast<AEnemyCharacter>(OverlappingEnemies[i]))
		{
			AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OverlappingEnemies[i]);
			enemy->SetCurrentState(EEnemyState::Triggered);
		}
	}
}

void AEnemyCharacter::SpawnBloodEffects(FHitResult HitResult, AActor* SourceActor)
{
	// Blood decals
	if (BloodDecal != NULL)
	{
		for (int i = 0; i < 10; i++)
		{
			// Blood Baloons
			UWorld* const World = GetWorld();
			if (World)
			{
				FVector Normal = HitResult.Normal * -1;
				FRotator Rotation = Normal.Rotation();
				Normal = Normal.RotateAngleAxis(FMath::RandRange(0.0, 20.0f), FVector(FMath::RandRange(0.0, 1.0f), FMath::RandRange(0.0, 1.0f), FMath::RandRange(0.0, 1.0f)));
				ABloodParticleBall* ball = World->SpawnActor<ABloodParticleBall>(ABloodParticleBall::StaticClass(), HitResult.Location, HitResult.Normal.Rotation());
				ball->Decal = BloodDecal;
				ball->GetProjectileMovement()->InitialSpeed = 1000.0f;
				ball->LifetimeDestroy = 1.5f;
				//ball->CollisionComp->MoveIgnoreActors.Add(this);
				ball->CollisionComp->IgnoreActorWhenMoving(this, true);
				ball->GetProjectileMovement()->ProjectileGravityScale = 5.0;
				//Debug::LogOnScreen("spwaning blood");
			}
		}
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnTakeDamage();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DamageTaken"));
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	if (CurrentHealth <= 0)
	{
		bIsAlive = false;
		Death();
		UStatsPornManager::IncreaseAmountOfEnemiesKilled();
	}

	return DamageAmount;
}

void AEnemyCharacter::AddDelayedImpulse(FVector Impulse, FVector Location)
{
	DelayedImpulses.Add({ Impulse, Location });
}

void AEnemyCharacter::Death()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	UMusicManager::RemoveEnemy(EnemyState);
}

void AEnemyCharacter::SetCurrentState(EEnemyState State)
{
	UMusicManager::OnEnemyStateSwitch(EnemyState, State);
	EnemyState = State;
}

void AEnemyCharacter::OnTakeDamage_Implementation()
{

}