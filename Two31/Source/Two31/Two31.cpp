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
