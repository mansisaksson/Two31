#include "Two31.h"
#include "PlayerCharacter.h"
#include "Utilities/Weapon.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	SprintMultiplier = 1.5f;
	CurrentHealth = 50.f;
	MaxHealth = 100.f;
	CurrentArmor = 35.f;
	MaxArmor = 100.f;

	ClipSize = 32;
	CurrentAmmo = 32;
	ReserveAmmo = 100;
	MaxAmmo = 200;

	bIsSprinting = false;
	bFireIsPressed = false;
	bCanJump = true;

	// Create a CameraComponent	
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPCamera->AttachParent = GetCapsuleComponent();
	FPCamera->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FPCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FPArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FPArmMesh->SetOnlyOwnerSee(true);
	FPArmMesh->AttachParent = FPCamera;
	FPArmMesh->bCastDynamicShadow = false;
	FPArmMesh->CastShadow = false;
}

void APlayerCharacter::BeginPlay()
{
	if (StarterWeapon1 != NULL)
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FVector SpawnLocation = FVector::ZeroVector;
		WeaponSlot1 = GetWorld()->SpawnActor<AWeapon>(StarterWeapon1, SpawnLocation, SpawnRotation);
		WeaponSlot1->AttachRootComponentTo(FPArmMesh, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		WeaponSlot1->SetActorHiddenInGame(true);
	}
	if (StarterWeapon2 != NULL)
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FVector SpawnLocation = FVector::ZeroVector;
		WeaponSlot2 = GetWorld()->SpawnActor<AWeapon>(StarterWeapon2, SpawnLocation, SpawnRotation);
		WeaponSlot2->AttachRootComponentTo(FPArmMesh, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		WeaponSlot2->SetActorHiddenInGame(true);
	}
	if (StarterWeapon3 != NULL)
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FVector SpawnLocation = FVector::ZeroVector;
		WeaponSlot3 = GetWorld()->SpawnActor<AWeapon>(StarterWeapon3, SpawnLocation, SpawnRotation);
		WeaponSlot3->AttachRootComponentTo(FPArmMesh, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		WeaponSlot3->SetActorHiddenInGame(true);
	}
	if (StarterWeapon4 != NULL)
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FVector SpawnLocation = FVector::ZeroVector;
		WeaponSlot4 = GetWorld()->SpawnActor<AWeapon>(StarterWeapon4, SpawnLocation, SpawnRotation);
		WeaponSlot4->AttachRootComponentTo(FPArmMesh, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
		WeaponSlot4->SetActorHiddenInGame(true);
	}
	SelectWeaponSlot1();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	if (bFireIsPressed && CurrentWeapon != NULL)
		CurrentWeapon->UpdateFire(DeltaSeconds, FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	/*to remove*/
	if (bCanJump)
	{
		InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	}

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnReleaseFire);

	InputComponent->BindAction("WeaponSlot1", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot1);
	InputComponent->BindAction("WeaponSlot2", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot2);
	InputComponent->BindAction("WeaponSlot3", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot3);
	InputComponent->BindAction("WeaponSlot4", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot4);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveSideways);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::OnFire()
{
	bFireIsPressed = true;
	if (CurrentWeapon != NULL)
		CurrentWeapon->StartFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
}

void APlayerCharacter::OnReleaseFire()
{
	bFireIsPressed = false;
	if (CurrentWeapon != NULL)
		CurrentWeapon->StopFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
}

void APlayerCharacter::SelectWeaponSlot1()
{
	if (WeaponSlot1 != NULL && CurrentWeapon != WeaponSlot1)
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlot1;
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetArmAnimations(FPArmMesh);
	}
}
void APlayerCharacter::SelectWeaponSlot2()
{
	if (WeaponSlot2 != NULL && CurrentWeapon != WeaponSlot2)
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlot2;
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetArmAnimations(FPArmMesh);
	}
}
void APlayerCharacter::SelectWeaponSlot3()
{
	if (WeaponSlot3 != NULL && CurrentWeapon != WeaponSlot3)
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlot3;
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetArmAnimations(FPArmMesh);
	}
}
void APlayerCharacter::SelectWeaponSlot4()
{
	if (WeaponSlot4 != NULL && CurrentWeapon != WeaponSlot4)
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlot4;
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetArmAnimations(FPArmMesh);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (!bIsSprinting || Value < 0)
			Value /= SprintMultiplier;

		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveSideways(float Value)
{
	if (Value != 0.0f)
	{
		Value /= SprintMultiplier;
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::StartSprint()
{
	bIsSprinting = true;
}

void APlayerCharacter::StopSprint()
{
	bIsSprinting = false;
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

float APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

float APlayerCharacter::GetMaxHealth()
{
	return MaxHealth;
}

float APlayerCharacter::GetArmor()
{
	return CurrentArmor;
}

float APlayerCharacter::GetMaxArmor()
{
	return MaxArmor;
}

int32 APlayerCharacter::GetClipSize()
{
	return ClipSize;
}

int32 APlayerCharacter::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int32 APlayerCharacter::GetReserveAmmo()
{
	return ReserveAmmo;
}

int32 APlayerCharacter::GetMaxAmmo()
{
	return MaxAmmo;
}