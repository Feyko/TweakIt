#pragma once
#include "lib/lua.h"
#include "TweakIt/TweakItModule.h"

class FScript;

class FRunnableScript : public FRunnable
{
public:
	FString FileName;
	bool Waiting;
	FString EventWaitedFor;
	int* Result;
	
	explicit FRunnableScript(FScript* Script);
	
	virtual uint32 Run() override;

private:
	lua_State* L;
};

enum class EScriptCompletion
{
	Ok,
	Error,
};

class FScript
{
	friend FRunnableScript;
public:
	explicit FScript(lua_State* L,FString FileName);
	
	FString FileName;
	int Result = -1;
	
	int Run();
private:
	lua_State* L;
	FRunnableScript Script;
	FRunnableThread* Thread = nullptr;
};

