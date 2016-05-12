// Copyright 2014 Allegorithmic All Rights Reserved.

#pragma once

#include "ISubstanceCore.h"
#include "Ticker.h"

// Settings
#include "SubstanceSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "SubstanceModule"


class FSubstanceCoreModule : public ISubstanceCore
{
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool Tick(float DeltaTime);

	void RegisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "Substance",
				LOCTEXT("SubstanceSettingsName", "Substance"),
				LOCTEXT("SubstanceSettingsDescription", "Configure the Substance plugin"),
				GetMutableDefault<USubstanceSettings>()
				);
		}
	}

	void UnregisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "Substance");
		}
	}

#if WITH_EDITOR
	void OnBeginPIE(const bool bIsSimulating);
	void OnEndPIE(const bool bIsSimulating);

public:
	bool isPie(){ return PIE; }
#endif //WITH_EDITOR

private:
	FTickerDelegate TickDelegate;

	static void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);
	static void OnLevelAdded(ULevel* Level, UWorld* World);

	void LoadSubstanceLibraries();
	
	bool PIE;
};

#undef LOCTEXT_NAMESPACE
