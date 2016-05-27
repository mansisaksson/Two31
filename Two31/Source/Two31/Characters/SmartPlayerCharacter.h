// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/PlayerCharacter.h"
#include "SmartPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TWO31_API ASmartPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = SmartShit)
	void MoveForward2(float Value) { APlayerCharacter::MoveForward(Value); }
	
	UFUNCTION(BlueprintCallable, Category = SmartShit)
	void MoveSideways2(float Value) { APlayerCharacter::MoveSideways(Value); }

};
