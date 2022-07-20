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
	
	static FString MakeEventForMod(FString ModReference, FString Lifecycle = "Module");
	template<typename... T>
	static FString MakeEventString(T... EventParts);
	
	template<typename... T>
	bool HasEventPassed(T... EventParts);
	bool HasModPassed(FString ModReference, FString Lifecycle = "Module");

	bool ResumeForMod(FString ModReference, FString Lifecycle = "Module");

	template<typename... T>
	bool ResumeScriptsWaitingForEvent(bool Unique, T... EventParts);

	static TArray<FString> GetAllScripts();
	
	static FString GetConfigDirectory();
	static FTIScriptOrchestrator* Get();
private:
	static void CreateDefaultScript();
	void SetupModEvents();
	
	TArray<FScript*> RunningScripts;
	TArray<FString> PassedUniqueEvents;
};
