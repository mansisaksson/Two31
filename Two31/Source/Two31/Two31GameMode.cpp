// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Two31.h"
#include "Two31GameMode.h"
#include "Two31HUD.h"
#include "Two31Character.h"

ATwo31GameMode::ATwo31GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATwo31HUD::StaticClass();
}
