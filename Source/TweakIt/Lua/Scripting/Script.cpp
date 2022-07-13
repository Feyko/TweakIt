#include "Script.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "ModuleDescriptor.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Subsystem/TIScriptOrchestrator.h"

FScript::FScript(FString FileName) : FileName(FileName), State(FScriptState::NotRan), L(FLuaState())
{
	PrettyName = PrettyFilename(FileName);
}

FScriptState FScript::Start()
{
	LOG("Starting script")
	FTILog::CurrentScript = PrettyName;
	State = FScriptState::Running;
	int Returned = luaL_dofile(L.L, TCHAR_TO_UTF8(*FileName));

	FScriptState NewState = FScriptState::Successful;
	if (Returned != LUA_OK)
	{
		FString ErrorMsg = lua_tostring(L.L, -1);
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