#pragma once

#include "Characters/EnemyCharacter.h"
#include "MusicManager.generated.h"

/*
	Assumes int32 array is initialized to all zeroes.
*/

UCLASS()
class TWO31_API UMusicManager : public UObject
{
public:

	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Default")
	static void OnEnemyStateSwitch(EEnemyState CurrentState, EEnemyState NewState);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static void RemoveEnemy(EEnemyState CurrentState);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static void AddEnemy(EEnemyState CurrentState);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static int32 GetNumberOfEnemiesInState(EEnemyState State);

	static const int32 NumberOfStates = static_cast<int32>(EEnemyState::NumberOfStates) - 1;
	static int32 StateCounter[];
	static bool FirstTime;

};