#ifndef __TWO31_H__
#define __TWO31_H__

#include "EngineMinimal.h"
#include "Engine.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(DebugLog, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(DebugInit, Log, All);

//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(DebugAI, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(DebugError, Log, All);

//Use Example: UE_LOG(DebugLog, Log, TEXT("Can Now Fire"));

static class Debug
{
public:
	static void LogOnScreen(FString message, FColor color = FColor::White);
	static void Log(FString message);
	static void LogWarning(FString message);
	static void LogFatalError(FString message);

private:
	Debug() {}
};

#endif
