#include "Two31.h"
#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	Health = 100.f;
	bIsChasingPlayer = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);

	NavSystem = GetWorld()->GetNavigationSystem();
	AIController = Cast<AAIController>(GetController());

	if (AIController == NULL)
		UE_LOG(DebugError, Fatal, TEXT("AIController Not found!"));
}

void AEnemyCharacter::BeginPlay()
{

}

void AEnemyCharacter::Tick(float DeltaTime)
{

}

void AEnemyCharacter::InflictDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, 100.f);
}

void AEnemyCharacter::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + OtherActor->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TODO: game-specific logic
}

void AEnemyCharacter::OnSeePawn(APawn *OtherPawn)
{
	if (!bIsChasingPlayer)
	{
		NavSystem->SimpleMoveToActor(GetController(), OtherPawn);
		bIsChasingPlayer = true;
	}
}

void AEnemyCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEnemyCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEnemyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEnemyCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

float AEnemyCharacter::GetHealth()
{
	return Health;
}