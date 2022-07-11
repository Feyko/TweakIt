#pragma once
#include "ScriptState.h"
#include "TweakIt/Lua/LuaState.h"
#include "TweakIt/Lua/lib/lua.h"

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

	static FString PrettyFilename(FString ScriptFilename);
private:
	
	FScriptState State;
};