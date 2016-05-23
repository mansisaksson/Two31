// Fill out your copyright notice in the Description page of Project Settings.

#include "Two31.h"
#include "Confetti.h"


// Sets default values
AConfetti::AConfetti()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConfetti::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConfetti::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

float AConfetti::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0;
}

