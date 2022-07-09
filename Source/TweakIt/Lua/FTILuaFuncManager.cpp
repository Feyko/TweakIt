#include "FTILuaFuncManager.h"

#include "Buildables/FGBuildableFactoryBuilding.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"

TMap<FString, FLuaFunc> FTILuaFuncManager::Funcs = {};

FLuaFunc::FLuaFunc(lua_State* L) : L(L)
{
	Buf = new TArray<uint8>;
}

void FLuaFunc::Free()
{
	delete Buf;
}

bool FLuaFunc::AddData(const void* NewData, size_t DataSize)
{
	Buf->Append(static_cast<const uint8*>(NewData), DataSize);
	return true;
}

const char* FLuaFunc::GetData()
{
	return reinterpret_cast<const char*>(Buf->GetData());
}

size_t FLuaFunc::Size()
{
	return Buf->Num();
}

FLuaFunc FTILuaFuncManager::DumpFunction(lua_State* L, FString Name, int Index, bool Strip)
{
	LOGF("Trying to dump %s", *Name)
	if (!lua_isfunction(L, Index) || lua_iscfunction(L, Index))
	{
		luaL_argexpected(L, false, Index, "Lua Function");
	}
	LOG("Proceeding with dumping")
	lua_settop(L, lua_gettop(L) + 1);
	lua_copy(L, Index, -1);
	FLuaFunc Desc = FLuaFunc(L);
	lua_dump(L, WriterFunc, &Desc, Strip);
	lua_settop(L, -2);
	Funcs.Add(Name, Desc);
	return Desc;
}

int FTILuaFuncManager::LoadFunction(lua_State* L, FLuaFunc Func, FString Name)
{
	LOGF("Loading function %s", *Name)
	return luaL_loadbuffer(L, Func.GetData(), Func.Size(), TCHAR_TO_UTF8(*Name));
}

int FTILuaFuncManager::LoadSavedFunction(lua_State* L, FString Name)
{
	FLuaFunc* Func = GetSavedFunction(Name);
	if (Func == nullptr)
	{
		return -1;
	}
	return LoadFunction(L, *Func, Name);
}

FLuaFunc* FTILuaFuncManager::GetSavedFunction(FString Name)
{
	return Funcs.Find(Name);
}

FNativeFuncPtr FTILuaFuncManager::SavedLuaFuncToNativeFunc(lua_State* L, FString Name)
{
	LOG("Making a native func out of a saved lua func")
	FLuaFunc* LuaFunc = GetSavedFunction(Name);
	if (LuaFunc == nullptr)
	{
		return nullptr;
	}
	return [](UObject* Context, FFrame& Frame, void* const)
	{
		if (Frame.Node == nullptr)
		{
			LOGL("Node was null", Error)
			return;
		}
		FLuaFunc* LuaFunc = GetSavedFunction(Frame.Node->GetName());
		if (LuaFunc == nullptr)
		{
			LOGFL("Could not find Lua func %s", Error, *Frame.Node->GetName())
			return;
		}
		LoadFunction(LuaFunc->L, *LuaFunc, Frame.Node->GetName());
		for (auto Prop = Frame.Node->PropertyLink; Prop; Prop = Prop->PropertyLinkNext)
		{
			FTILua::PropertyToLua(LuaFunc->L, Prop, Frame.Locals);
		}
		int r = lua_pcall(LuaFunc->L, Frame.Node->NumParms, Frame.Node->GetReturnProperty() != nullptr, 0);
		if (r != LUA_OK)
		{
			LOGFL("Errored when calling func %s", Error, *Frame.Node->GetName())
		}
	};
}

int FTILuaFuncManager::WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor)
{
	FLuaFunc* Desc = static_cast<FLuaFunc*>(Descriptor);
	return !Desc->AddData(NewData, DataSize);
}
