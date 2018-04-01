#include "Pickup.h"
#include "Two31.h"
#include "../DefaultGameMode.h"
#include "Components/SphereComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetMobility(EComponentMobility::Movable);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetRootComponent(ItemMesh);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	TriggerSphere->InitSphereRadius(50.0f);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerSphere->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerSphere->CanCharacterStepUpOn = ECB_No;

	TriggerSphere->bGenerateOverlapEvents = true;
	TriggerSphere->SetupAttachment(ItemMesh);

	BobHeight = 1.f;
	BobSpeed = 3.f;
	RotSpeed = 10.f;
}

void APickup::BeginPlay() 
{
	Super::BeginPlay();
	DefaultGameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode());
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BobTimer += DeltaTime;
	if (DefaultGameMode != NULL)
	{
		if (DefaultGameMode->GetConfig()->GameplayProggMode)
		{
			//Debug::LogOnScreen(FString::Printf(TEXT("Sin: %f"), FMath::Sin(BobTimer)));
			SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, FMath::Sin(BobTimer * BobSpeed) * BobHeight));

			FRotator Rot = GetActorRotation();
			Rot.Yaw += RotSpeed * DeltaTime;
			SetActorRotation(Rot);
		}
	}
}
