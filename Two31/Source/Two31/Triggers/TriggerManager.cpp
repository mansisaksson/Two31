#include "TriggerManager.h"
#include "Two31.h"
#include "Components/BoxComponent.h"

ATriggerManager::ATriggerManager()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//TriggerBox->InitBoxExtent()
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->CanCharacterStepUpOn = ECB_No;

	TriggerBox->bGenerateOverlapEvents = true;
}
