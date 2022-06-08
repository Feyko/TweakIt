#pragma once
#include "LuaState.h"
#include "lib/lua.h"
#include "TweakIt/TweakItModule.h"

class FScript;

class FRunnableScript : public FRunnable
{
public:
	virtual ~FRunnableScript() override;
	FString FileName;
	bool Waiting;
	FString EventWaitedFor;
	int* Result;
	
	explicit FRunnableScript(FScript* Script);
	
	virtual uint32 Run() override;
	virtual bool Init() override;
	virtual void Stop() override;
	virtual void Exit() override;
private:
	lua_State* L = nullptr;
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
	explicit FScript(FString FileName);
	
	FString FileName;
	FLuaState* L = nullptr;
	int Result = -1;
	
	int Run();
private:
	FRunnableScript Script;
	FRunnableThread* Thread = nullptr;
};

