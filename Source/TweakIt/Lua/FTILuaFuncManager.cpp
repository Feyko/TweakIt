#include "FTILuaFuncManager.h"

#include "Buildables/FGBuildableFactoryBuilding.h"
#include "TweakIt/Logging/FTILog.h"

TMap<FString, FLuaFunc> FTILuaFuncManager::Funcs = {};

FLuaFunc::FLuaFunc()
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
	FLuaFunc Desc;
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
	LOG("Did find the lua func")
	return [](UObject* Context, FFrame& Frame, void* const)
	{
		LOG("CALLING MADE NATIVE FUNC")
		LOG(Frame.Node->GetFullName())
		LOG(Frame.Node->GetSuperFunction()->GetFullName())
		LOG(Frame.Node->GetSuperFunction()->NumParms)
		if (Frame.Node == nullptr)
		{
			LOG("Node was null")
			return;
		}
		LOG(Frame.Node->NumParms)
		for (auto Prop = Frame.Node->PropertyLink; Prop; Prop->PropertyLinkNext)
		{
			LOG(Prop->GetFullName())
			LOG(Prop->ElementSize)
		}
	};
}

int FTILuaFuncManager::WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor)
{
	FLuaFunc* Desc = static_cast<FLuaFunc*>(Descriptor);
	return !Desc->AddData(NewData, DataSize);
}
