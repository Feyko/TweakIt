#pragma once
#include "LuaState.h"
#include "lib/lua.h"

class FScriptState
{
public:
	enum EScriptState
	{
		Running,
		Waiting,
		Successful,
		Errored,
	};

	FString Payload;

	bool IsCompleted();
private:
	EScriptState V;

	// Bullshit needed for this to work as an enum
public:
	FScriptState() = default;
	FScriptState(EScriptState State) : V(State) { }
	explicit constexpr operator EScriptState() const { return V; }
	explicit operator bool() const = delete;    
	bool operator==(FScriptState Other) const { return V == Other.V; }
	bool operator!=(FScriptState Other) const { return V != Other.V; }
	bool operator==(EScriptState Other) const { return V == Other; }
	bool operator!=(EScriptState Other) const { return V != Other; }
};

DECLARE_MULTICAST_DELEGATE_OneParam(FScriptStateDelegate, FScriptState)
DECLARE_MULTICAST_DELEGATE(FScriptCompletedDelegate)

class FScript;

class FRunnableScript : public FRunnable
{
public:
	explicit FRunnableScript(FScript* Script);
	
	virtual uint32 Run() override;
private:
	FScript* Script;
	lua_State* L;
};

class FScript
{
	friend FRunnableScript;
public:
	explicit FScript(FString FileName);
	
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
	
	FRunnableScript Script;
	FRunnableThread* Thread = nullptr;
};

