// Copyright 2014 Allegorithmic All Rights Reserved.

#include "SubstanceCorePrivatePCH.h"
#include "SubstanceCoreHelpers.h"
#include "SubstanceCoreModule.h"

#include "SubstanceCoreClasses.h"

#include "AssetRegistryModule.h"
#include "ModuleManager.h"
#include "Ticker.h"

namespace
{
	static FWorldDelegates::FWorldInitializationEvent::FDelegate OnWorldInitDelegate;
	static FDelegateHandle OnWorldInitDelegateHandle;

	static FWorldDelegates::FOnLevelChanged::FDelegate OnLevelAddedDelegate;
	static FDelegateHandle OnLevelAddedDelegateHandle;
}

void FSubstanceCoreModule::StartupModule()
{
	Substance::Helpers::SetupSubstance();

	// Register tick function.
	TickDelegate = FTickerDelegate::CreateRaw(this, &FSubstanceCoreModule::Tick);
	FTicker::GetCoreTicker().AddTicker( TickDelegate );

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	RegisterSettings();

	::OnWorldInitDelegate = FWorldDelegates::FWorldInitializationEvent::FDelegate::CreateStatic(&FSubstanceCoreModule::OnWorldInitialized);
	::OnWorldInitDelegateHandle = FWorldDelegates::OnPostWorldInitialization.Add(::OnWorldInitDelegate);
	
	::OnLevelAddedDelegate = FWorldDelegates::FOnLevelChanged::FDelegate::CreateStatic(&FSubstanceCoreModule::OnLevelAdded);
	::OnLevelAddedDelegateHandle = FWorldDelegates::LevelAddedToWorld.Add(::OnLevelAddedDelegate);

#if WITH_EDITOR
	FEditorDelegates::BeginPIE.AddRaw(this, &FSubstanceCoreModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FSubstanceCoreModule::OnEndPIE);
#endif //WITH_EDITOR

	PIE = false;
}


void FSubstanceCoreModule::ShutdownModule()
{
	FWorldDelegates::OnPostWorldInitialization.Remove(::OnWorldInitDelegateHandle);
	FWorldDelegates::LevelAddedToWorld.Remove(::OnLevelAddedDelegateHandle);

	UnregisterSettings();

	if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	}

	Substance::Helpers::TearDownSubstance();
}


bool FSubstanceCoreModule::Tick(float DeltaTime)
{
	Substance::Helpers::Tick();

	return true;
}


void FSubstanceCoreModule::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	Substance::Helpers::OnWorldInitialized();
}

void FSubstanceCoreModule::OnLevelAdded(ULevel* Level, UWorld* World)
{
	Substance::Helpers::OnLevelAdded();
}

#if WITH_EDITOR
void FSubstanceCoreModule::OnBeginPIE(const bool bIsSimulating)
{
	PIE = true;
}

void FSubstanceCoreModule::OnEndPIE(const bool bIsSimulating)
{
	PIE = false;
}
#endif

IMPLEMENT_MODULE( FSubstanceCoreModule, SubstanceCore );
