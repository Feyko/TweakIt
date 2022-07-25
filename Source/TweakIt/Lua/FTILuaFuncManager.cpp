#include "FTILuaFuncManager.h"

#include "Buildables/FGBuildableFactoryBuilding.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"

TMap<FString, FLuaFunc> FTILuaFuncManager::SavedLuaFuncs = {};

FLuaFunc::FLuaFunc(lua_State* L) : L(L)
{
	
}

bool FLuaFunc::AddData(const void* NewData, size_t DataSize)
{
	Buf.Append(static_cast<const uint8*>(NewData), DataSize);
	return true;
}

const char* FLuaFunc::GetData()
{
	return reinterpret_cast<const char*>(Buf.GetData());
}

size_t FLuaFunc::Size()
{
	return Buf.Num();
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
	TResult<FLuaFunc> Func = GetSavedLuaFunc(Name);
	if (!Func)
	{
		return -1;
	}
	return LoadFunction(L, *Func, Name);
}

TResult<FLuaFunc> FTILuaFuncManager::GetSavedLuaFunc(FString Name)
{
	FLuaFunc* Func = SavedLuaFuncs.Find(Name);
	if(Func == nullptr)
	{
		return Errors::Newf("function %s was not saved", *Name);
	}
	return *Func;
}

FNativeFuncPtr FTILuaFuncManager::SavedLuaFuncToNativeFunc(lua_State* L, FString Name)
{
	LOG("Making a native func out of a saved lua func")
	TResult<FLuaFunc> LuaFunc = GetSavedLuaFunc(Name);
	if (!LuaFunc)
	{
		LOGFL("Could not find the saved function: %s", Warning, *LuaFunc.Err()->Error())
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
	TResult<FLuaFunc> LuaFunc = GetSavedLuaFunc(Frame.Node->GetName());
	if (!LuaFunc)
	{
		LOGFL("Could not find Lua func %s", Error, *Frame.Node->GetName())
		return;
	}
	lua_State* L = LuaFunc->L;
	FTILua::StackDump(L);
	
	LoadFunction(L, *LuaFunc, Frame.Node->GetName());
	LOG("Pushing context object")
	FLuaUObject::ConstructObject(L, Context);
	FTILua::StackDump(L);
	for (auto Prop = Frame.Node->PropertyLink; Prop; Prop = Prop->PropertyLinkNext)
	{
		FTILua::PropertyToLua(L, Prop, Frame.Locals);
	}
	FProperty* ReturnProperty = Frame.Node->GetReturnProperty();
	LOG("Calling inner Lua func")
	int r = lua_pcall(L, Frame.Node->NumParms + 1, ReturnProperty != nullptr, 0);
	if (r != LUA_OK)
	{
		FString Error = lua_tostring(L, -1);
		LOGFL("Errored when calling func %s: %s", Error, *Frame.Node->GetName(), *Error)
	}
	if (ReturnProperty)
	{
		LOG("Copying return value")
		FTILua::LuaToProperty(L, ReturnProperty, Result, -1);
	}
	LOG("Done")
}
