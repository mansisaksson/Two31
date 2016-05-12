// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "StatsPornManager.generated.h"

/**
 * 
 */
UCLASS()
class TWO31_API UStatsPornManager : public UObject
{
	GENERATED_BODY()
	
public:
	UStatsPornManager();

	UFUNCTION(BlueprintCallable, Category = "Filler")
		static void IncreaseFiller() { Filler++; }
	UFUNCTION(BlueprintCallable, Category = "Filler")
		static int32 GetFiller() { return Filler; }

	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfEnemies() { AmountOfEnemies++; }
	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfEnemiesKilled() { AmountOfEnemiesKilled++; }

	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfEnemies() { Debug::LogOnScreen(FString::Printf(TEXT("Amount of Enemies %i"), AmountOfEnemiesKilled)); return AmountOfEnemies; }
	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfEnemiesKilled() { return AmountOfEnemiesKilled; }

	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfSecrets() { AmountOfSecrets++; }
	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfSecretsFound() { AmountOfSecretsFound++; }

	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfSecrets() { return AmountOfSecrets; }
	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfSecretsFound() { return AmountOfSecretsFound; }

	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfShotsFired() { AmountOfShotsFired++; }

	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfShotsFired() { return AmountOfShotsFired; }

	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfDamageTaken() { AmountOfDamageTaken++; }
	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfHealthLost() { AmountOfHealthLost++; }
	UFUNCTION(BlueprintCallable, Category = "IncreaseStatFunction")
		static void IncreaseAmountOfArmorLost() { AmountOfArmorLost++; }

	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfDamageTaken() { return AmountOfDamageTaken; }
	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfHealthLost() { return AmountOfHealthLost; }
	UFUNCTION(BlueprintCallable, Category = "GetFunction")
		static int32 GetAmountOfArmorLost() { return AmountOfArmorLost; }

	// Timer

	UFUNCTION(BlueprintCallable, Category = "Clear")
		static void ClearStats();

private:
	static int32 Filler;

	static int32 AmountOfEnemies;
	static int32 AmountOfEnemiesKilled;

	static int32 AmountOfSecrets;
	static int32 AmountOfSecretsFound;

	static int32 AmountOfShotsFired;

	static int32 AmountOfDamageTaken;
	static int32 AmountOfHealthLost;
	static int32 AmountOfArmorLost;

};