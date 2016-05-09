#include "Two31.h"
#include "PlayerCharacter.h"
#include "../Pickups/Pickup.h"
#include "../Pickups/ItemPickup.h"
#include "../Pickups/WeaponPickup.h"
#include "../Pickups/HealthPickup.h"
#include "../Pickups/ArmorPickup.h"
#include "../Pickups/AmmoPickup.h"
#include "../Characters/EnemyCharacter.h"
#include "../Weapons/Weapon.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	ViewPitchMax = 70.f;
	ViewPitchMin = -70.f;
	DefaultFOV = 90.f;
	ADSFOV = 60.f;
	
	MeleeDamage = 50.f;
	MeleePowah = 90000.f;
	CurrentHealth = 50.f;
	MaxHealth = 100.f;
	CurrentArmor = 35.f;
	MaxArmor = 100.f;
	MaxAmountOfHealthPacks = 3;
	LastFootstep = 0.f;
	MeleeTime = 1.666667;
	TimeSinceMelee = 0.f;

	IndicatorLocation = 0.f;
	IndicatorTimer = 0.f;
	IndicatorDisplayTime = 2.f;
	IndicatorOpacity = 0.f;

	WeaponSlots.SetNum(4);
	HealthPacks.SetNum(0);
	Items.SetNum(0);
	Inventory.SetNum(0);

	bFireIsPressed = false;
	bCanJump = false;
	bADS = false;
	bMeleeAttack = false;

	// Create a CameraComponent	
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPCamera->AttachParent = GetCapsuleComponent();
	FPCamera->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FPCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FPArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FPArmMesh->SetOnlyOwnerSee(false);
	FPArmMesh->AttachParent = FPCamera;
	FPArmMesh->bCastDynamicShadow = false;
	FPArmMesh->CastShadow = false;

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeCollider"));
	MeleeCollider->AttachTo(FPArmMesh, TEXT("L_Wrist"));
	MeleeCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	MeleeCollider->SetSphereRadius(30.f);
	MeleeCollider->IgnoreActorWhenMoving(this, true);
	MeleeCollider->bGenerateOverlapEvents = true;
	MeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnMeleeBeginOverlap);

	LineOfSight_Chest = CreateDefaultSubobject<USceneComponent>(TEXT("LineOfSight_Chest"));
	LineOfSight_Chest->AttachTo(GetCapsuleComponent());
	LineOfSight_Chest->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

	LineOfSight_Shoulder_Right = CreateDefaultSubobject<USceneComponent>(TEXT("LineOfSight_Shoulder_Right"));
	LineOfSight_Shoulder_Right->AttachTo(GetCapsuleComponent());
	LineOfSight_Shoulder_Right->SetRelativeLocation(FVector(0.f, 40.f, 60.f));

	LineOfSight_Shoulder_Left = CreateDefaultSubobject<USceneComponent>(TEXT("LineOfSight_Shoulder_Left"));
	LineOfSight_Shoulder_Left->AttachTo(GetCapsuleComponent());
	LineOfSight_Shoulder_Left->SetRelativeLocation(FVector(0.f, -40.f, 60.f));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FPCamera->FieldOfView = DefaultFOV;

	PlayerController = Cast<APlayerController>(Controller);
	PlayerController->PlayerCameraManager->ViewPitchMax = ViewPitchMax;
	PlayerController->PlayerCameraManager->ViewPitchMin = ViewPitchMin;

	DefaultArmLocation = FPArmMesh->GetRelativeTransform().GetLocation();
	DefaultArmRotation = FPArmMesh->GetRelativeTransform().Rotator();

	DefaultMeleeRadius = MeleeCollider->GetUnscaledSphereRadius();
	SetMeleeRadius(0.f);

	EquipWeapon(StarterWeapon1);
	EquipWeapon(StarterWeapon2);
	EquipWeapon(StarterWeapon2);
	EquipWeapon(StarterWeapon4);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bFireIsPressed && CurrentWeapon != NULL)
	{
		if (bMeleeAttack && TimeSinceMelee == 0.f)
			CurrentWeapon->StopFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
		else
			CurrentWeapon->UpdateFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
	}
		

	FVector VelocityVector = GetCharacterMovement()->Velocity;
	float VectorMagnitude = VelocityVector.Size();

	float Now = GetWorld()->GetTimeSeconds();
	if (Now > LastFootstep + 1.0f && VectorMagnitude > 0.f && !GetCharacterMovement()->IsCrouching())
	{
		MakeNoise(1.0f, this, FVector::ZeroVector);
		LastFootstep = Now;
	}

	if (CurrentWeapon != NULL)
	{
		if (bADS && !CurrentWeapon->GetIsReloading())
		{
			FPCamera->FieldOfView = FMath::Lerp(FPCamera->FieldOfView, ADSFOV, 10.f * DeltaSeconds);
			FPArmMesh->SetRelativeRotation(FMath::Lerp(FPArmMesh->GetRelativeTransform().Rotator(), FRotator(3.f, -108.2f, 0.f), 10.f * DeltaSeconds));
			FPArmMesh->SetRelativeLocation(FMath::Lerp(FPArmMesh->GetRelativeTransform().GetLocation(), FVector(0.f, -30.65f, -148.2f), 10.f * DeltaSeconds));
		}
		else
		{
			FPCamera->FieldOfView = FMath::Lerp(FPCamera->FieldOfView, DefaultFOV, 10.f * DeltaSeconds);
			FPArmMesh->SetRelativeRotation(FMath::Lerp(FPArmMesh->GetRelativeTransform().Rotator(), DefaultArmRotation, 10.f * DeltaSeconds));
			FPArmMesh->SetRelativeLocation(FMath::Lerp(FPArmMesh->GetRelativeTransform().GetLocation(), DefaultArmLocation, 10.f * DeltaSeconds));
		}
	}
	else
		FPCamera->FieldOfView = FMath::Lerp(FPCamera->FieldOfView, DefaultFOV, 10.f * DeltaSeconds);
	
	if (bMeleeAttack)
	{
		TimeSinceMelee += DeltaSeconds;
		if (TimeSinceMelee >= MeleeTime)
		{
			TimeSinceMelee = 0.f;
			bMeleeAttack = false;
			MeleedActors.Empty();
		}
	}

	if (IndicatorTimer > 0.f)
	{
		IndicatorTimer -= DeltaSeconds;
		IndicatorOpacity = (IndicatorTimer/IndicatorDisplayTime);
	}

}

void APlayerCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<APickup>(OtherActor))
	{
		if (Cast<AWeaponPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Weapon Pickup"));
			AWeaponPickup* WeaponPickup = Cast<AWeaponPickup>(OtherActor);
			if (EquipWeapon(WeaponPickup->GetWeapon()))
			{
				WeaponPickup->Destroy();
				PickedUpItem(OtherActor);
			}
		}
		else if (Cast<AHealthPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health Pickup"));
			AHealthPickup* Healthpack = Cast<AHealthPickup>(OtherActor);
			if (PickupHealthPack(Healthpack))
			{
				Healthpack->Destroy();
				PickedUpItem(OtherActor);
			}

		}
		else if (Cast<AArmorPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Armor Pickup"));
			AArmorPickup* Armor = Cast<AArmorPickup>(OtherActor);
			if (ChangeArmor(Armor->GetArmor()))
			{
				Armor->Destroy();
				PickedUpItem(OtherActor);
			}

		}
		else if (Cast<AAmmoPickup>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Ammo Pickup"));
			AAmmoPickup* Ammo = Cast<AAmmoPickup>(OtherActor);
			if (AddAmmo(Ammo->GetAmmoType(), Ammo->GetAmount()))
			{
				Ammo->Destroy();
				PickedUpItem(OtherActor);
			}
		}
		else if (Cast<AItemPickup>(OtherActor))
		{
			Debug::LogOnScreen(TEXT("Item Pickup"));
			AItemPickup* Item = Cast<AItemPickup>(OtherActor);
			if (AddItem(Item))
				Item->Destroy();
		}
	}
}

void APlayerCharacter::OnMeleeBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!MeleedActors.Contains(OtherActor))
	{
		FVector Distance = OtherActor->GetActorLocation() - GetActorLocation();
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		OtherActor->TakeDamage(MeleeDamage, DamageEvent, OtherActor->GetInstigatorController(), this);

		FVector Angle = OtherComp->GetComponentLocation() - GetActorLocation();
		Angle.Normalize();
		if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor))
			enemy->AddDelayedImpulse(Angle * MeleePowah, enemy->GetActorLocation());

		else if (OtherComp->Mobility == EComponentMobility::Movable && OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulse(Angle * MeleePowah);
	}
	MeleedActors.Add(OtherActor);
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
	InputComponent->BindAction("Reload", IE_Released, this, &APlayerCharacter::OnReload);
	InputComponent->BindAction("ADS", IE_Pressed, this, &APlayerCharacter::OnADS);
	InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &APlayerCharacter::OnMeleeAttack);

	InputComponent->BindAction("WeaponSlot1", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot1);
	InputComponent->BindAction("WeaponSlot2", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot2);
	InputComponent->BindAction("WeaponSlot3", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot3);
	InputComponent->BindAction("WeaponSlot4", IE_Pressed, this, &APlayerCharacter::SelectWeaponSlot4);

	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &APlayerCharacter::NextWeapon);
	InputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &APlayerCharacter::PreviousWeapon);

	InputComponent->BindAction("UseHealthPack", IE_Pressed, this, &APlayerCharacter::UseHealthPack);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveSideways);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::OnFire()
{
	if (!bMeleeAttack)
	{
		bFireIsPressed = true;
		if (CurrentWeapon != NULL)
			CurrentWeapon->StartFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
	}

}
void APlayerCharacter::OnReleaseFire()
{
	bFireIsPressed = false;
	if (CurrentWeapon != NULL)
		CurrentWeapon->StopFire(FPCamera->GetComponentLocation() + (GetControlRotation().Vector() * 5000.f));
}
void APlayerCharacter::OnReload()
{
	if (CurrentWeapon != NULL)
		CurrentWeapon->Reload();
}
void APlayerCharacter::OnADS()
{
	bADS = !bADS;
}
void APlayerCharacter::OnMeleeAttack()
{
	if (!bMeleeAttack)
	{
		bMeleeAttack = true;
		TimeSinceMelee = 0.f;
		Debug::LogOnScreen("Meele!");
	}
}

bool APlayerCharacter::PickupHealthPack(AHealthPickup* Healthpack)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth = FMath::Clamp((CurrentHealth + Healthpack->GetHealth()), 0.f, MaxHealth);
		return true;
	}
	else if (CurrentHealth == MaxHealth && HealthPacks.Num() < MaxAmountOfHealthPacks)
	{
		HealthPacks.Add(Healthpack->GetHealth());
		return true;
	}
	return false;
}
bool APlayerCharacter::ChangeArmor(float pChange)
{
	if (CurrentArmor < MaxArmor)
	{
		CurrentArmor = FMath::Clamp((CurrentArmor + pChange), 0.f, MaxArmor);
		return true;
	}
	return false;
}
bool APlayerCharacter::AddAmmo(EAmmoType Ammo, int Amount)
{
	FAmmo* AmmoToRefill;
	switch (Ammo)
	{
	case EAmmoType::BulletAmmo:
		AmmoToRefill = &BulletAmmo;
		break;
	case EAmmoType::ShotgunAmmo:
		AmmoToRefill = &ShotgunAmmo;
		break;
	case EAmmoType::ExplosiveAmmo:
		AmmoToRefill = &ExplosiveAmmo;
		break;
	case EAmmoType::PlasmaAmmo:
		AmmoToRefill = &PlasmaAmmo;
		break;
	default:
		AmmoToRefill = NULL;
		break;
	}
	if (AmmoToRefill != NULL && (AmmoToRefill->AmmoPool < AmmoToRefill->MaxAmmo))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Refill ammo"));
		AmmoToRefill->AmmoPool = FMath::Clamp(AmmoToRefill->AmmoPool + Amount, 0, AmmoToRefill->MaxAmmo);
		return true;
	}

	return false;
}
bool APlayerCharacter::AddItem(AItemPickup* item)
{
	if (Inventory.Num() < 4 && Inventory.Num() > 0 )
	{
		bool  bShouldAdd = true;
		for (size_t i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i].ID == item->GetItemID())
				bShouldAdd = false;
		}
		if (bShouldAdd)
		{
			SInventory toAdd;
			toAdd.ID = item->GetItemID();
			toAdd.Name = item->GetItemName();
			Inventory.Add(toAdd);
			return true;
		}
	}
	else if (Inventory.Num() == 0)
	{
		SInventory toAdd;
		toAdd.ID = item->GetItemID();
		toAdd.Name = item->GetItemName();
		Inventory.Add(toAdd);
		return true;
	}
	return false;
}
int32 APlayerCharacter::GetItem(int32 itemID)
{
	if (Inventory.Num() == 0)
		return 0;
	if (Inventory.Num() > itemID)
		return Inventory[itemID].ID;

	return 0;
}
bool APlayerCharacter::PlayerHasItem(int32 ItemID)
{
	for (size_t i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ID == ItemID)
			return true;
	}

	return false;
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
				WeaponSlots[i]->AttachRootComponentTo(FPArmMesh);
				WeaponSlots[i]->HolsterWeapon();

				if (CurrentWeapon == NULL)
					SelectWeaponSlot(i);
				return true;
			}
		}
	}
	return false;
}
void APlayerCharacter::SelectWeaponSlot(int index)
{
	if (WeaponSlots[index] != NULL && CurrentWeapon != WeaponSlots[index])
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->HolsterWeapon();

		CurrentWeapon = WeaponSlots[index];
		
		switch ((EAmmoType)CurrentWeapon->GetAmmoType())
		{
		case EAmmoType::BulletAmmo:
			CurrentAmmo = &BulletAmmo;
			break;
		case EAmmoType::ShotgunAmmo:
			CurrentAmmo = &ShotgunAmmo;
			break;
		case EAmmoType::ExplosiveAmmo:
			CurrentAmmo = &ExplosiveAmmo;
			break;
		case EAmmoType::PlasmaAmmo:
			CurrentAmmo = &PlasmaAmmo;
			break;
		default:
			CurrentAmmo = NULL;
			break;
		}

		if (CurrentAmmo != NULL)
			CurrentWeapon->EquipWeapon(FPArmMesh, &CurrentAmmo->AmmoPool);
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

void APlayerCharacter::NextWeapon()
{
	int index = GetWeaponIndex() + 1;
	if (index > WeaponSlots.Num() - 1)
		index = 0;

	int tempIndex = index;
	while (WeaponSlots[tempIndex] == NULL)
	{
		tempIndex++;
		if (tempIndex > WeaponSlots.Num() - 1)
			tempIndex = 0;
		if (tempIndex == index)
			break;
	}

	SelectWeaponSlot(tempIndex);
}
class AWeapon* APlayerCharacter::GetNextWeapon()
{
	int index = GetWeaponIndex() + 1;
	if (index > WeaponSlots.Num() - 1)
		index = 0;

	int tempIndex = index;
	while (WeaponSlots[tempIndex] == NULL)
	{
		tempIndex++;
		if (tempIndex > WeaponSlots.Num() - 1)
			tempIndex = 0;
		if (tempIndex == index)
			break;
	}
	
	return WeaponSlots[tempIndex];
}
void APlayerCharacter::PreviousWeapon()
{
	int index = GetWeaponIndex() - 1;
	if (index < 0)
		index = WeaponSlots.Num() - 1;

	int tempIndex = index;
	while (WeaponSlots[tempIndex] == NULL)
	{
		tempIndex--;
		if (tempIndex < 0)
			tempIndex = WeaponSlots.Num() - 1;
		if (tempIndex == index)
			break;
	}

	SelectWeaponSlot(tempIndex);
}
int APlayerCharacter::GetWeaponIndex()
{
	int index = -1;
	for (size_t i = 0; i < WeaponSlots.Num(); i++)
	{
		if (CurrentWeapon == WeaponSlots[i])
			index = i;
	}
	return index;
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorForwardVector(), Value);
}
void APlayerCharacter::MoveSideways(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::UseHealthPack()
{
	if (CurrentHealth < MaxHealth && HealthPacks.Num() > 0)
	{
		CurrentHealth = FMath::Clamp((CurrentHealth + HealthPacks[0]), 0.f, MaxHealth);
		HealthPacks.RemoveAt(0);
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, 100.f);
	//if (CurrentHealth == 0)
	//	bIsAlive = false;

	IndicatorLocation = GetDamageCauserLocation(DamageCauser);
	IndicatorTimer = IndicatorDisplayTime;

	return DamageAmount;
}

float APlayerCharacter::GetDamageCauserLocation(AActor* DamageCauser)
{
	FVector Direction = GetActorLocation() - (DamageCauser->GetActorLocation());
	Direction.Normalize();

	float DotProd = FVector::DotProduct(Direction, GetActorForwardVector());
	float DotProd2 = FVector::DotProduct(Direction, GetActorRightVector());

	float DotDegree1 = FMath::Acos(DotProd);
	float DotDegree2 = FMath::Acos(DotProd2);
	DotDegree1 = FMath::RadiansToDegrees(DotDegree1);
	DotDegree2 = FMath::RadiansToDegrees(DotDegree2);

	if (DotDegree2 < 90.f)
		DotDegree1 *= -1;

	DotDegree1 = 180 - DotDegree1;

	return DotDegree1;
}

void APlayerCharacter::PickedUpItem_Implementation(AActor* OtherActor)
{

}

int32 APlayerCharacter::GetAmountOfItems() { return Inventory.Num(); }
FString APlayerCharacter::GetItemName(AItemPickup* ItemToName) {  return ItemToName->GetItemName(); }
int32 APlayerCharacter::GetItemID(AItemPickup* ItemToName) { return ItemToName->GetItemID(); }

void APlayerCharacter::SetMeleeRadius(float Radius)
{
	if (Radius == 0.f)
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeleeCollider->SetSphereRadius(Radius);
}