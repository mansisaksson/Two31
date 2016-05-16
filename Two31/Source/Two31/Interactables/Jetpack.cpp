#include "Two31.h"
#include "Jetpack.h"
#include "../Characters/EnemyCharacter.h"

AJetpack::AJetpack()
	: AInteractable()
{
	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Jetpack"));
	SetRootComponent(JetMesh);

	UpArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrow"));
	UpArrow->AttachTo(JetMesh);
	bIsActive = false;
}

void AJetpack::BeginPlay()
{
	AInteractable::BeginPlay();
	bIsActive = false;
}

void AJetpack::Tick(float DeltaTime)
{
	AInteractable::Tick(DeltaTime);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetAttachParentActor()))
	{
		if (Enemy->GetHealth() <= 0.f)
			bIsActive = true;
	}

	if (bIsActive)
	{
		if (JetMesh->GetAttachParent() != NULL)
		{
			if (UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(JetMesh->GetAttachParent()))
				Comp->AddImpulse(UpArrow->GetComponentRotation().Vector() * ImpulsePowah);
		}
	}
}

USceneComponent* AJetpack::GetOwnerComp()
{
	return JetMesh->GetAttachParent();
}
