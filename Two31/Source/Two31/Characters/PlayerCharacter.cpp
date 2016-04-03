#include "Two31.h"
#include "PlayerCharacter.h"
#include "../Utilities/Pickups/Pickup.h"
#include "../Utilities/Pickups/WeaponPickup.h"
#include "../Utilities/Pickups/HealthPickup.h"
#include "../Utilities/Pickups/AmmoPickup.h"
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

	WeaponSlots.SetNum(4);

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
	EquipWeapon(StarterWeapon1);
	EquipWeapon(StarterWeapon2);
	EquipWeapon(StarterWeapon2);
	EquipWeapon(StarterWeapon4);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	if (bFireIsPressed && CurrentWeapon != NULL)
		CurrentWeapon->UpdateFire(DeltaSeconds, FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
}

void APlayerCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Cast<APickup>(OtherActor))
	{
		if (Cast<AWeaponPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Weapon Pickup"));
			AWeaponPickup* WeaponPickup = Cast<AWeaponPickup>(OtherActor);
			if (EquipWeapon(WeaponPickup->GetWeapon()))
				WeaponPickup->Destroy();
		}
		else if (Cast<AHealthPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health Pickup"));
		}
		else if (Cast<AAmmoPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Ammo Pickup"));
		}
	}
}

bool APlayerCharacter::EquipWeapon(TSubclassOf<AWeapon> Weapon)
{
	if (Weapon != NULL)
	{
		for (size_t i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i] == NULL)
			{
				const FRotator SpawnRotation = FRotator::ZeroRotator;
				const FVector SpawnLocation = FVector::ZeroVector;
				WeaponSlots[i] = GetWorld()->SpawnActor<AWeapon>(Weapon, SpawnLocation, SpawnRotation);
				WeaponSlots[i]->AttachRootComponentTo(FPArmMesh, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
				WeaponSlots[i]->SetActorHiddenInGame(true);

				if (CurrentWeapon == NULL)
					SelectWeaponSlot(i);
				return true;
			}
		}
	}
	return false;
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

void APlayerCharacter::SelectWeaponSlot(int index)
{
	if (WeaponSlots[index] != NULL && CurrentWeapon != WeaponSlots[index])
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlots[index];
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetArmAnimations(FPArmMesh);
	}
}
void APlayerCharacter::SelectWeaponSlot1()
{
	SelectWeaponSlot(0);
}
void APlayerCharacter::SelectWeaponSlot2()
{
	SelectWeaponSlot(1);
}
void APlayerCharacter::SelectWeaponSlot3()
{
	SelectWeaponSlot(2);
}
void APlayerCharacter::SelectWeaponSlot4()
{
	SelectWeaponSlot(3);
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