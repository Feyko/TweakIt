#include "Script.h"

#include "ModuleDescriptor.h"
#include "TweakIt/Logging/FTILog.h"

FScript::FScript(FString FileName) : FileName(FileName)
{
	StateChanged = FPlatformProcess::CreateSynchEvent();
	Script = new FRunnableScript(this);
	L.LifecycleNotifier->ScriptWaitingDelegate.AddLambda([this]
	{
		this->SetState(FScriptState::Waiting);
	});
	L.LifecycleNotifier->ScriptResumeDelegate.AddLambda([this]
	{
		this->SetState(FScriptState::Running);
	});
}

FScript::~FScript()
{
	Thread->Kill();
	delete Script;
}

void FScript::Start()
{
	Thread = FRunnableThread::Create(Script, *("TweakIt Script: " + this->FileName));
}

FScriptState FScript::WaitForStop()
{
	LOGF("Waiting for stop %ls", *FileName)
	while (State == FScriptState::Running)
	{
		StateChanged->Wait();
		LOGF("State changed %ls", *FileName)
	}
	LOGF("Stopped %ls", *FileName)
	return State;
}

FScriptState FScript::WaitForCompletion()
{
	Thread->WaitForCompletion();
	return State;
}

void FScript::SetState(FScriptState NewState)
{
	this->State = NewState;
	StateChanged->Trigger();
}

FScriptState FScript::GetState()
{
	return this->State;
}

FRunnableScript::FRunnableScript(FScript* Script) : Script(Script), L(Script->L.L)
{
	FTILog::CurrentScript = Script->FileName;
}

uint32 FRunnableScript::Run()
{
	int Returned = luaL_dofile(L, TCHAR_TO_UTF8(*Script->FileName));

	FScriptState StopReason = FScriptState::Successful;
	if (Returned != LUA_OK)
	{
		FString ErrorMsg = lua_tostring(L, -1);
		StopReason = FScriptState::Errored;
		StopReason.Payload = ErrorMsg;
	}
	Script->SetState(StopReason);
	return Returned;
}

void FRunnableScript::Stop()
{
	lua_close(L);
}
