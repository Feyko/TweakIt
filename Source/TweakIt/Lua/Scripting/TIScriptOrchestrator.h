#pragma once

#include "FGSubsystem.h"
#include "TweakIt/Lua/LuaState.h"
#include "TweakIt/Lua/Scripting/Script.h"

class FTIScriptOrchestrator
{
public:
	FTIScriptOrchestrator();
	~FTIScriptOrchestrator();
	
	bool StartAllScripts();

	FScriptState StartScript(FString Name);

	static TArray<FString> GetAllScripts();

	static void CreateDefaultScript();
	static FString GetConfigDirectory();

	static FTIScriptOrchestrator* Get();

	TArray<FScript*> RunningScripts;
};
