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

	bAggro = false;

	RotationTimer = 2.f;

	TargetLocation = FVector::ZeroVector;

	bIsChasingPlayer = false;
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

	if (bAttackOnCooldown)
	{
		TimeSinceLastAttack += DeltaTime;
		if (TimeSinceLastAttack > AttackCooldown)
		{
			bIsChasingPlayer = false;
			bAttackOnCooldown = false;
			TimeSinceLastAttack = 0.f;
		}
	}

	if (bAggro)
	{
		// Mask is used to mask the Z-Vector during rotation.
		FVector Mask = FVector(1, 1, 0);
		FVector MyLocation = GetMesh()->GetComponentLocation();

		FVector TargetRotation = (TargetLocation - MyLocation);
		TargetRotation *= Mask;
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
	NavSystem->SimpleMoveToLocation(GetController(), OtherPawn->GetActorLocation());
	//if (!bIsChasingPlayer)
	//{
	//	
	//	//NavSystem->SimpleMoveToActor(GetController(), OtherPawn);
	//	bIsChasingPlayer = true;
	//	//Attack(OtherPawn);
	//}
}

float AImpCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AEnemyCharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Herpa DERPA!!!"));
	
	TargetLocation = DamageCauser->GetActorLocation();
	if (!bAggro)
		bAggro = true;

	
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

void AImpCharacter::Death()
{
	AEnemyCharacter::Death();
	AttackRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}