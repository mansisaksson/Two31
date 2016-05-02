#include "Two31.h"
#include "Pickup.h"

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
	TriggerSphere->AttachTo(ItemMesh);
}