#include "Script.h"

#include "Lua.h"
#include "TweakIt/TweakItModule.h"

FScript::FScript(FString FileName) : FileName(FileName), Script(FRunnableScript(this))
{
	
}

int FScript::Run()
{
	// LOG("Running")
	Thread = FRunnableThread::Create(&Script, *("TweakIt Script: " + this->FileName));
	return Result;
}

bool FRunnableScript::Init()
{
	// LOG("INIT")
	return true;
}

void FRunnableScript::Stop()
{
	// LOG("STOP")
}

void FRunnableScript::Exit()
{
	// LOG("EXIT")
}

FRunnableScript::~FRunnableScript()
{
	// LOG("DESTRUCT")
}

FRunnableScript::FRunnableScript(FScript* Script) : FileName(Script->FileName), Waiting(false), Result(&Script->Result)
{
	// LOG("CONSTRUCT")
	// LOGFS(FileName)
	// LOGFS(this->FileName)
}

uint32 FRunnableScript::Run()
{
	// LOG("Actually running")
	// int r = luaL_dofile(L, TCHAR_TO_UTF8(*FileName));
	int ayo = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			ayo = FMath::Pow(ayo, i);
		}
	}
	LOG("done bro")
	LOGF("%d", ayo)
	// *Result = r;
	// LOGFS(FileName)
	// LOGFS(this->FileName)
	// LOG("Finished running")
	return 0;
}