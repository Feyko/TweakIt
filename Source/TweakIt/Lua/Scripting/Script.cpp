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
	LOG("Starting script")
	FTILog::CurrentScript = PrettyName;
	State = FScriptState::Running;
	luaL_loadfile(L.L, TCHAR_TO_UTF8(*FileName));
	lua_KFunction K = [](lua_State* L, int status, lua_KContext ctx) -> int
	{
		LOG("CONTINUING")
		return 0;
	};
	
	// int Returned = lua_pcallk(L.L, 0, LUA_MULTRET, 0, 0, K);
	int results = 0;
	int Returned = lua_resume(L.L, nullptr, 0, &results);
	FScriptState NewState = FScriptState::Successful;
	if (Returned == LUA_YIELD)
	{
		NewState = FScriptState::Waiting;
		NewState.Payload = L.EventWaitedFor;
		LOGF("Waiting on %s", *L.EventWaitedFor)
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

FString FScript::PrettyFilename(FString ScriptFilename)
{
	FPaths::NormalizeFilename(ScriptFilename);
	FPaths::MakePathRelativeTo(ScriptFilename, *FTIScriptOrchestrator::GetConfigDirectory());
	return ScriptFilename;
}