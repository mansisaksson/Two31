#include "Two31.h"
#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "../MusicManager.h"
#include "../Characters/PlayerCharacter.h"
#include "ImpCharacter.h"
#include "AIController.h"
#include "Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	MaxHealth = 100.f;
	bIsAlive = true;

	DespawnTimer = 3.f;
	TimeSinceDeath = 0.f;

	BloodDecalMinSize = 40.f;
	BloodDecalMaxSize = 100.f;

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

void AEnemyCharacter::BloodEffects()
{
	// Blood splat particles
	if (BloodParticle != NULL)
	{
		UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(BloodParticle, GetMesh() , TEXT("None"), GetActorLocation(), GetActorRotation().GetNormalized() * -1.f, EAttachLocation::KeepWorldPosition);
		ParticleSystemComp->AddLocalRotation(FRotator(90.f, 0.f, 0.f));
	}

	// Blood decals
	if (BloodDecal != NULL)
	{
		FHitResult result;
		ECollisionChannel collisionChannel;
		collisionChannel = ECC_WorldDynamic;
		FCollisionQueryParams collisionQuery;
		collisionQuery.bTraceComplex = true;
		FCollisionObjectQueryParams objectCollisionQuery;
		FCollisionResponseParams collisionResponse;
		collisionResponse = ECR_Block;

		collisionQuery.AddIgnoredActor(this);
		collisionQuery.AddIgnoredActor(GetOwner());
		//collisionQuery.AddIgnoredActor(HitResult.GetActor());

		bool hitObject = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + FVector(0.f, 0.f, -1000.f), collisionChannel, collisionQuery, collisionResponse);

		if (hitObject)
		{
			float DecalSize = (BloodDecalMaxSize - BloodDecalMinSize) * FMath::FRand() + BloodDecalMinSize;
			UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BloodDecal, FVector(DecalSize, DecalSize, 1.f), result.Location, result.Normal.Rotation() * -1.f);
			DecalComp->AddRelativeRotation(FRotator(0.f, 0.f, FMath::FRand() * 360.f));
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
		bIsAlive = false;
	BloodEffects();
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

	for (size_t i = 0; i < DelayedImpulses.Num(); i++) {
		Debug::LogOnScreen(FString::Printf(TEXT("Add Impulse! | Strength: %f"), DelayedImpulses[i].Impulse.Size()));
		GetMesh()->AddImpulseAtLocation(DelayedImpulses[i].Impulse, DelayedImpulses[i].Location);
	}
	DelayedImpulses.Empty();
}

void AEnemyCharacter::SetCurrentState(EEnemyState State)
{
	UMusicManager::OnEnemyStateSwitch(EnemyState, State);
	EnemyState = State;
}

void AEnemyCharacter::OnTakeDamage_Implementation()
{

}