#pragma once
#include "LuaState.h"
#include "lib/lua.h"
#include "TweakIt/TweakItModule.h"

enum class EScriptStopState
{
	Not,
	Completed,
	Errored,
	Waiting,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FScriptStoppedDelegate, EScriptStopState)

class FScript;

class FRunnableScript : public FRunnable
{
public:
	FString FileName;
	FScriptStoppedDelegate Delegate;
	
	explicit FRunnableScript(FScript* Script);
	
	virtual uint32 Run() override;
private:
	lua_State* L;
};

class FScript
{
	friend FRunnableScript;
public:
	explicit FScript(FString FileName);
	
	FString FileName;
	EScriptStopState StopReason = EScriptStopState::Not;
	FLuaState L = FLuaState(nullptr);

	void Start();
	EScriptStopState WaitForStop();
private:
	FRunnableScript Script;
	FRunnableThread* Thread = nullptr;
};

