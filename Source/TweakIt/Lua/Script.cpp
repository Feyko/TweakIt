#include "Script.h"

#include "Lua.h"
#include "TweakIt/TweakItModule.h"

FScript::FScript(lua_State* L, FString FileName) : L(L), FileName(FileName), Script(FRunnableScript(this))
{
	
}

int FScript::Run()
{
	Thread = Thread->Create(&Script, *("TweakIt Script: " + this->FileName));
	Thread->WaitForCompletion();
	return Result;
}

FRunnableScript::FRunnableScript(FScript* Script) : FileName(Script->FileName), Waiting(false), L(Script->L), Result(&Script->Result) {}

uint32 FRunnableScript::Run()
{
	int r = luaL_dofile(L, TCHAR_TO_UTF8(*FileName));
	*Result = r;
	return r;
}