// Fill out your copyright notice in the Description page of Project Settings.

#include "StatsPornManager.h"
#include "Two31.h"

//Initialize all Stats to 0
int32 UStatsPornManager::Filler = 0;
int32 UStatsPornManager::AmountOfEnemies = 0;
int32 UStatsPornManager::AmountOfEnemiesKilled = 0;
int32 UStatsPornManager::AmountOfSecrets = 0;
int32 UStatsPornManager::AmountOfSecretsFound = 0;
int32 UStatsPornManager::AmountOfShotsFired = 0;
int32 UStatsPornManager::AmountOfDamageTaken = 0;
int32 UStatsPornManager::AmountOfHealthLost = 0;
int32 UStatsPornManager::AmountOfArmorLost = 0;

UStatsPornManager::UStatsPornManager()
{

}

void UStatsPornManager::ClearStats()
{
	Filler = 0;
	AmountOfEnemies = 0;
	AmountOfEnemiesKilled = 0;
	AmountOfSecrets = 0;
	AmountOfSecretsFound = 0;
	AmountOfShotsFired = 0;
	AmountOfDamageTaken = 0;
	AmountOfHealthLost = 0;
	AmountOfArmorLost = 0;
}

