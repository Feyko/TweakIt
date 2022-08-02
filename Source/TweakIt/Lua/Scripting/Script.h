#pragma once
#include "ScriptState.h"
#include "TweakIt/Lua/LuaState.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FScriptStateDelegate, FScriptState)
DECLARE_MULTICAST_DELEGATE(FScriptCompletedDelegate)

class FScript
{
public:
	explicit FScript(FString FileName);

	FString FileName;
	FString PrettyName;
	FLuaState L;

	FScriptState Start();
	FScriptState Resume();

	static FString PrettyFilename(FString ScriptFilename);

	FScriptState GetState() { return State;}
private:
	FScriptState Run();
	
	FScriptState State;
};