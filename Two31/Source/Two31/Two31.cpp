#include "Two31.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Two31, "Two31" );

//General Log
DEFINE_LOG_CATEGORY(DebugLog);

//Logging during game startup
DEFINE_LOG_CATEGORY(DebugInit);

//Logging for your AI system
DEFINE_LOG_CATEGORY(DebugAI);

//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(DebugError);

void Debug::LogOnScreen(FString message)
{
	LogOnScreen(message, 2.5f, FColor::White);
}
void Debug::LogOnScreen(FString message, FColor color)
{
	LogOnScreen(message, 2.5f, color);
}
void Debug::LogOnScreen(FString message, float screenTime, FColor color)
{
	GEngine->AddOnScreenDebugMessage(-1, screenTime, color, message);
}
void Debug::Log(FString message)
{
	UE_LOG(DebugLog, Log, TEXT("%s"), *message);
}
void Debug::LogWarning(FString message)
{
	UE_LOG(DebugLog, Warning, TEXT("%s"), *message);
}
void Debug::LogFatalError(FString message)
{
	UE_LOG(DebugError, Error, TEXT("%s"), *message);
	//UKismetSystemLibrary::QuitGame(GetWorld(), )
}
