﻿#include "Script.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "ModuleDescriptor.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Subsystem/TweakItSubsystem.h"

FScript::FScript(FString FileName) : FileName(FileName)
{
	PrettyName = FormatFilename(FileName);
	StateChanged = FPlatformProcess::CreateSynchEvent();
	Ready = FPlatformProcess::CreateSynchEvent();
	Script = new FRunnableScript(this);
}

FScript::~FScript()
{
	Thread->Kill();
	delete Script;
}

void FScript::SetLuaState(FLuaState* NewState)
{
	L = NewState;
	SetCallbacks();
	Ready->Trigger();
}

void FScript::SetCallbacks()
{
	L->LifecycleNotifier->ScriptWaitingDelegate.AddLambda([this]
	{
		this->SetState(FScriptState::Waiting);
	});
	L->LifecycleNotifier->ScriptResumeDelegate.AddLambda([this]
	{
		this->SetState(FScriptState::Running);
	});
}

void FScript::Start()
{
	Thread = FRunnableThread::Create(Script, *("TweakIt Script: " + this->FileName));
	Ready->Wait();
}

FScriptState FScript::WaitForStop()
{
	while (State == FScriptState::Running)
	{
		StateChanged->Wait();
	}
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

FString FScript::FormatFilename(FString ScriptFilename)
{
	FPaths::NormalizeFilename(ScriptFilename);
	FPaths::MakePathRelativeTo(ScriptFilename, *ATweakItSubsystem::GetConfigDirectory());
	return ScriptFilename;
}

FRunnableScript::FRunnableScript(FScript* Script) : Script(Script)
{
}

void FRunnableScript::InitState()
{
	if (L != nullptr)
	{
		return;
	}
	
	L = new FLuaState(nullptr);
	Script->SetLuaState(L);
}

bool FRunnableScript::Init()
{
	FTILog::CurrentScript = Script->PrettyName;
	InitState();
	return true;
}

uint32 FRunnableScript::Run()
{
	InitState();
	int Returned = luaL_dofile(L->L, TCHAR_TO_UTF8(*Script->FileName));

	FScriptState StopReason = FScriptState::Successful;
	if (Returned != LUA_OK)
	{
		FString ErrorMsg = lua_tostring(L->L, -1);
		StopReason = FScriptState::Errored;
		StopReason.Payload = ErrorMsg;
	}
	Script->SetState(StopReason);
	return Returned;
}

void FRunnableScript::Stop()
{
	lua_close(L->L);
}
