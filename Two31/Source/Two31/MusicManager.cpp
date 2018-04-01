#include "MusicManager.h"
#include "Two31.h"

bool UMusicManager::FirstTime = true;
int32 UMusicManager::StateCounter[NumberOfStates];

void UMusicManager::OnEnemyStateSwitch(EEnemyState CurrentState, EEnemyState NewState)
{
	//Debug::LogOnScreen("StateSwitch");
	StateCounter[static_cast<int32>(CurrentState)] = FMath::Clamp(StateCounter[static_cast<int32>(CurrentState)] - 1, 0, 999999999);
	StateCounter[static_cast<int32>(NewState)]++;
}

void UMusicManager::RemoveEnemy(EEnemyState CurrentState)
{
	//Debug::LogOnScreen("Remove Enemy");
	StateCounter[static_cast<int32>(CurrentState)] = FMath::Clamp(StateCounter[static_cast<int32>(CurrentState)] - 1, 0, 999999999);
}

void UMusicManager::AddEnemy(EEnemyState CurrentState)
{
	if (FirstTime) {
		FirstTime = false;
		ClearBuffer();
	}
	StateCounter[static_cast<int32>(CurrentState)]++;
}

int32 UMusicManager::GetNumberOfEnemiesInState(EEnemyState State)
{
	return StateCounter[static_cast<int32>(State)];
}

void UMusicManager::ClearBuffer()
{
	for (int i = 0; i < NumberOfStates; i++) {
		StateCounter[i] = 0;
	}
}