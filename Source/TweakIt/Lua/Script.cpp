#include "Script.h"

#include "Lua.h"
#include "ModuleDescriptor.h"
#include "TweakIt/TweakItModule.h"

FScript::FScript(FString FileName) : FileName(FileName), Script(FRunnableScript(this)) {}

void FScript::Start()
{
	Thread = FRunnableThread::Create(&Script, *("TweakIt Script: " + this->FileName));
	Script.Delegate.AddLambda([this](EScriptStopState StopReason)
	{
		this->StopReason = StopReason;
	});
}

EScriptStopState FScript::WaitForStop()
{
	while (StopReason == EScriptStopState::Not)
	{
		FPlatformProcess::Sleep(0);
	}
	return StopReason;
}

FRunnableScript::FRunnableScript(FScript* Script) : FileName(Script->FileName), L(Script->L.L) {}

uint32 FRunnableScript::Run()
{
	int Returned = luaL_dofile(L, TCHAR_TO_UTF8(*FileName));

	EScriptStopState StopReason = EScriptStopState::Completed;
	if (Returned != LUA_OK)
	{
		FString ErrorMsg = lua_tostring(L, -1);
		LOGFS(ErrorMsg)
		StopReason = EScriptStopState::Errored;
	}
	Delegate.Broadcast(StopReason);
	return Returned;
}