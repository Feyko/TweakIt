#include "Script.h"

#include "ModuleDescriptor.h"
#include "TweakIt/TweakItModule.h"

FScript::FScript(FString FileName) : FileName(FileName)
{
	Script = new FRunnableScript(this);
}

FScript::~FScript()
{
	delete Script;
}

void FScript::Start()
{
	Thread = FRunnableThread::Create(Script, *("TweakIt Script: " + this->FileName));
}

FScriptState FScript::WaitForStop()
{
	while (State == FScriptState::Running)
	{
		FPlatformProcess::Sleep(0);
	}
	return State;
}

FScriptState FScript::WaitForCompletion()
{
	while (!State.IsCompleted())
	{
		FPlatformProcess::Sleep(0);
	}
	return State;
}

void FScript::Completed(FScriptState EndState)
{
	State = EndState;
	CompletedDelegate.Broadcast(EndState);
	StoppedDelegate.Broadcast(EndState);
}

FRunnableScript::FRunnableScript(FScript* Script) : Script(Script), L(Script->L.L) {}

uint32 FRunnableScript::Run()
{
	int Returned = luaL_dofile(L, TCHAR_TO_UTF8(*Script->FileName));

	FScriptState StopReason = FScriptState::Successful;
	if (Returned != LUA_OK)
	{
		FString ErrorMsg = lua_tostring(L, -1);
		LOGFS(ErrorMsg)
		StopReason = FScriptState::Errored;
	}
	Script->Completed(StopReason);
	return Returned;
}