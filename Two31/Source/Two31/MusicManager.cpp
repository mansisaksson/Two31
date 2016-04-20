
#include "Two31.h"
#include "MusicManager.h"

MusicManager::MusicManager()
{
	
}

MusicManager::~MusicManager()
{

}

void MusicManager::OnEnemyStateSwitch(EEnemyState CurrentState, EEnemyState NewState)
{
	StateCounter[static_cast<int32>(CurrentState)]--;
	StateCounter[static_cast<int32>(NewState)]++;
}

void MusicManager::RemoveEnemy(EEnemyState CurrentState)
{
	StateCounter[static_cast<int32>(CurrentState)]--;
}

void MusicManager::AddEnemy(EEnemyState CurrentState)
{
	StateCounter[static_cast<int32>(CurrentState)]++;
}