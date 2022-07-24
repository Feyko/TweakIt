#include "FTILuaFuncManager.h"

#include "Buildables/FGBuildableFactoryBuilding.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"

TMap<FString, FLuaFunc> FTILuaFuncManager::SavedLuaFuncs = {};

FLuaFunc::FLuaFunc(lua_State* L) : L(L)
{
	Buf = new TArray<uint8>;
}

// TODO: Properly manage script lifecycle and ownership
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
	SavedLuaFuncs.Add(Name, Desc);
	return Desc;
}

int FTILuaFuncManager::LoadFunction(lua_State* L, FLuaFunc Func, FString Name)
{
	LOGF("Loading function %s", *Name)
	return luaL_loadbuffer(L, Func.GetData(), Func.Size(), TCHAR_TO_UTF8(*Name));
}

int FTILuaFuncManager::LoadSavedFunction(lua_State* L, FString Name)
{
	FLuaFunc* Func = GetSavedLuaFunc(Name);
	if (Func == nullptr)
	{
		return -1;
	}
	return LoadFunction(L, *Func, Name);
}

FLuaFunc* FTILuaFuncManager::GetSavedLuaFunc(FString Name)
{
	return SavedLuaFuncs.Find(Name);
}

FNativeFuncPtr FTILuaFuncManager::SavedLuaFuncToNativeFunc(lua_State* L, FString Name)
{
	LOG("Making a native func out of a saved lua func")
	FLuaFunc* LuaFunc = GetSavedLuaFunc(Name);
	if (LuaFunc == nullptr)
	{
		LOG("Could not find the saved function")
		return nullptr;
	}
	LOG("could find")
	return LuaCallerFunc;
}

int FTILuaFuncManager::WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor)
{
	FLuaFunc* Desc = static_cast<FLuaFunc*>(Descriptor);
	return !Desc->AddData(NewData, DataSize);
}

void FTILuaFuncManager::LuaCallerFunc(UObject* Context, FFrame& Frame, void* const Result)
{
	if (Frame.Node == nullptr)
	{
		LOGL("Trying to call a LuaFunc but the Node was null", Error)
		return;
	}
	LOG("Calling wrapper function around Lua function")
	FLuaFunc* LuaFunc = GetSavedLuaFunc(Frame.Node->GetName());
	if (LuaFunc == nullptr)
	{
		LOGFL("Could not find Lua func %s", Error, *Frame.Node->GetName())
		return;
	}
	FTILua::StackDump(LuaFunc->L);
	LoadFunction(LuaFunc->L, *LuaFunc, Frame.Node->GetName());
	LOG("Pushing context object")
	FLuaUObject::ConstructObject(LuaFunc->L, Context);
	FTILua::StackDump(LuaFunc->L);
	for (auto Prop = Frame.Node->PropertyLink; Prop; Prop = Prop->PropertyLinkNext)
	{
		FTILua::PropertyToLua(LuaFunc->L, Prop, Frame.Locals);
	}
	FProperty* ReturnProperty = Frame.Node->GetReturnProperty();
	LOG("Calling inner Lua func")
	int r = lua_pcall(LuaFunc->L, Frame.Node->NumParms + 1, ReturnProperty != nullptr, 0);
	if (r != LUA_OK && r != LUA_YIELD)
	{
		FString Error = lua_tostring(LuaFunc->L, -1);
		LOGFL("Errored when calling func %s: %s", Error, *Frame.Node->GetName(), *Error)
	}
	if (ReturnProperty)
	{
		LOG("Copying return value")
		FTILua::LuaToProperty(LuaFunc->L, ReturnProperty, Result, -1);
	}
	LOG("Done")
}
