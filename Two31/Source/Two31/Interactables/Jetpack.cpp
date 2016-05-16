#include "Two31.h"
#include "Jetpack.h"

AJetpack::AJetpack()
	: AInteractable()
{
	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Jetpack"));
	SetRootComponent(JetMesh);

	UpArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrow"));
	UpArrow->AttachTo(UpArrow);
}

void AJetpack::BeginPlay()
{
	AInteractable::BeginPlay();
}

void AJetpack::Tick(float DeltaTime)
{
	AInteractable::Tick(DeltaTime);

	if (bIsActive)
	{
		if (JetMesh->GetAttachParent() != NULL)
		{
			if (UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(JetMesh->GetAttachParent()))
			{
				Comp->AddImpulse(FVector::ZeroVector);
			}
		}
	}
}

USceneComponent* AJetpack::GetOwnerComp()
{
	return JetMesh->GetAttachParent();
}
