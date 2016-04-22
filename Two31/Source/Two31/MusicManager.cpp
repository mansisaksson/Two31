#include "Two31.h"
#include "MusicManager.h"

bool UMusicManager::FirstTime = true;
int32 UMusicManager::StateCounter[NumberOfStates];

void UMusicManager::OnEnemyStateSwitch(EEnemyState CurrentState, EEnemyState NewState)
{
	StateCounter[static_cast<int32>(CurrentState)]--;
	StateCounter[static_cast<int32>(NewState)]++;
}

void UMusicManager::RemoveEnemy(EEnemyState CurrentState)
{
	StateCounter[static_cast<int32>(CurrentState)]--;
}

void UMusicManager::AddEnemy(EEnemyState CurrentState)
{
	if (FirstTime) {
		FirstTime = false;
		for (int i = 0; i < NumberOfStates; i++) {
			StateCounter[i] = 0;
		}
	}
	StateCounter[static_cast<int32>(CurrentState)]++;
}

int32 UMusicManager::GetNumberOfEnemiesInState(EEnemyState State)
{
	return StateCounter[static_cast<int32>(State)];
}