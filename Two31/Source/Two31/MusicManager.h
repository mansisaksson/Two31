#pragma once

#include "Characters/EnemyCharacter.h"

/*
	Assumes int32 array is initialized to all zeroes.
*/

class TWO31_API MusicManager
{

protected:
	static const int32 NumberOfStates = static_cast<int32>(EEnemyState::NumberOfStates) - 1;
	static int32 StateCounter[NumberOfStates];

public:
	MusicManager();
	~MusicManager();

	static void OnEnemyStateSwitch(EEnemyState CurrentState, EEnemyState NewState);
	static void RemoveEnemy(EEnemyState CurrentState);
	static void AddEnemy(EEnemyState CurrentState);
	static int32 GetNumberOfEnemiesInState(EEnemyState State) { return StateCounter[static_cast<int32>(State)]; }

};