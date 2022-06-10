#pragma once
#include "ScriptState.h"
#include "TweakIt/Lua/LuaState.h"
#include "TweakIt/Lua/lib/lua.h"

class FRunnableScript;
DECLARE_MULTICAST_DELEGATE_OneParam(FScriptStateDelegate, FScriptState)
DECLARE_MULTICAST_DELEGATE(FScriptCompletedDelegate)

class FScript
{
	friend FRunnableScript;
public:
	explicit FScript(FString FileName);
	~FScript();
	
	FString FileName;
	FScriptState State = FScriptState::Running;
	FLuaState L = FLuaState(nullptr);

	FScriptStateDelegate StoppedDelegate;
	FScriptStateDelegate CompletedDelegate;

	void Start();
	FScriptState WaitForStop();
	FScriptState WaitForCompletion();
private:
	void Completed(FScriptState EndState);
	
	FRunnableScript* Script;
	FRunnableThread* Thread = nullptr;
};

class FRunnableScript : public FRunnable
{
public:
	explicit FRunnableScript(FScript* Script);
	
	virtual uint32 Run() override;
	virtual void Stop() override;
private:
	FScript* Script;
	lua_State* L;
};