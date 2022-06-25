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
	FString PrettyName;
	FLuaState* L = nullptr;

	FEvent* StateChanged;

	void Start();
	FScriptState WaitForStop();
	FScriptState WaitForCompletion();

	void SetState(FScriptState NewState);
	FScriptState GetState();

	static FString PrettyFilename(FString ScriptFilename);
private:
	void SetLuaState(FLuaState* State);
	void SetCallbacks();
	FEvent* Ready;
	
	FScriptState State = FScriptState::Running;

	FRunnableScript* Script;
	FRunnableThread* Thread = nullptr;
};

class FRunnableScript : public FRunnable
{
public:
	explicit FRunnableScript(FScript* Script);

	void InitState();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
private:
	FScript* Script;
	FLuaState* L = nullptr;
};
