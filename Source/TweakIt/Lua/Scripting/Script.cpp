#include "Script.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "ModuleDescriptor.h"
#include "Interfaces/IPluginManager.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/Scripting/TIScriptOrchestrator.h"

FScript::FScript(FString FileName) : FileName(FileName), State(FScriptState::NotRan), L(FLuaState())
{
	PrettyName = PrettyFilename(FileName);
}

FScriptState FScript::Start()
{
	if (State != FScriptState::NotRan)
	{
		return State;
	}
	luaL_loadfile(L.L, TCHAR_TO_UTF8(*FileName));
	return Run();
}

FScriptState FScript::Resume()
{
	if (State != FScriptState::Waiting)
	{
		return State;
	}
	return Run();
}

FString FScript::PrettyFilename(FString ScriptFilename)
{
	FPaths::NormalizeFilename(ScriptFilename);
	FPaths::MakePathRelativeTo(ScriptFilename, *FTIScriptOrchestrator::GetConfigDirectory());
	return ScriptFilename;
}

FScriptState FScript::Run()
{
	FTILog::CurrentScript = PrettyName;
	State = FScriptState::Running;
	
	int NResults = 0;
	int Returned = lua_resume(L.L, nullptr, 0, &NResults);
	FScriptState NewState = FScriptState::Successful;
	if (Returned == LUA_YIELD)
	{
		lua_pop(L.L, NResults);
		if (NResults != 0)
		{
			LOGFL("Discarded %d results that were yielded", Warning, NResults)
		}
		NewState = FScriptState::Waiting;
		if (L.PlatformEventWaitedFor)
		{
			LOG("Waiting on platform event")
		} else
		{
			NewState.Payload = L.EventWaitedFor;
			LOGF("Waiting on %s", *L.EventWaitedFor)
		}
	}
	else if (Returned != LUA_OK)
	{
		FString ErrorMsg = luaL_checkstring(L.L, -1);
		NewState = FScriptState::Errored;
		NewState.Payload = ErrorMsg;
		LOGL(ErrorMsg, Error)
	}
	FTILog::CurrentScript = "";
	State = NewState;
	return NewState;
}
