#include "LuaFDelegate.h"
#include <string>

#include "TweakIt/TweakItTesting.h"
#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/FTILuaFuncManager.h"
using namespace std;

int FLuaFDelegate::Construct(lua_State* L, UFunction* SignatureFunction, FScriptDelegate* Delegate)
{
	if (!SignatureFunction->IsValidLowLevel() || !Delegate)
	{
		LOG("Trying to construct a LuaFDelegate from an invalid signature function or delegate")
		lua_pushnil(L);
		return 1;
	}
	LOG("Constructing a LuaFDelegate")
	FLuaFDelegate* ReturnedInstance = static_cast<FLuaFDelegate*>(lua_newuserdata(L, sizeof(FLuaFDelegate)));
	new(ReturnedInstance) FLuaFDelegate{SignatureFunction, Delegate};
	luaL_getmetatable(L, FLuaFDelegate::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaFDelegate* FLuaFDelegate::Get(lua_State* L, int Index)
{
	return static_cast<FLuaFDelegate*>(luaL_checkudata(L, Index, Name));
}

FString FLuaFDelegate::ToString() const
{
	return Delegate->ToString<UObject>();
}

int FLuaFDelegate::Lua_Bind(lua_State* L)
{
	LOG("Binding a LuaFDelegate")
    FLuaFDelegate* Self = Get(L);
	FTILua::LuaT_ExpectLuaFunction(L, 2);

	// TODO: Extract the following into function
	FString FunctionName = UTIUFunctionBinder::MakeFunctionName(FTILog::CurrentScript, Self->SignatureFunction->GetName());
	LOG("Dumping Lua func")
	FTILuaFuncManager::DumpFunction(L, FunctionName, 2);
	LOG("Copying UFunction")
	UFunction* Function = FTIReflection::CopyUFunction(Self->SignatureFunction, FunctionName);
	LOG("Making native func from lua func")
	FNativeFuncPtr Func = FTILuaFuncManager::SavedLuaFuncToNativeFunc(L, FunctionName);
	LOG("Setting native func")
	Function->SetNativeFunc(Func);
	LOG("Adding function")
	UTIUFunctionBinder::AddFunction(Function, FunctionName);
	LOG("Binding function")
	Self->Delegate->BindUFunction(UTIUFunctionBinder::Get(), FName(FunctionName));
	return 0;
}

int FLuaFDelegate::Lua_IsBound(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	lua_pushboolean(L, Self->Delegate->IsBound());
	return 1;
}

int FLuaFDelegate::Lua_Unbind(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	Self->Delegate->Unbind();
	return 0;
}

int FLuaFDelegate::Lua_Wait(lua_State* L)
{
	luaL_error(L, "WIP");
	FLuaFDelegate* Self = Get(L);
	return 0;
}

int FLuaFDelegate::Lua_Trigger(lua_State* L)
{
	LOG("Triggering LuaFDelegate")
	FLuaFDelegate* Self = Get(L);
	if (!Self->Delegate->IsBound())
	{
		LOGL("Tried to trigger unbound delegate", Warning)
		return 0;
	}
	UObject* Object = Self->Delegate->GetUObject();
	UFunction* Function = Object->FindFunction(Self->Delegate->GetFunctionName());
	if (!Function->IsValidLowLevel())
	{
		LOGL("Tried to trigger delegate bound to an invalid UFunction", Warning)
		return 0;
	}
	return FTILua::CallUFunction(L, Object, Function);
}

int FLuaFDelegate::Lua__index(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	const FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaFDelegate %s with %s", *Self->ToString(),*Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	return 0;
}

int FLuaFDelegate::Lua__call(lua_State* L)
{
	Lua_Trigger(L);
	return 0;
}

int FLuaFDelegate::Lua__tostring(lua_State* L)
{
	FLuaFDelegate* Self = static_cast<FLuaFDelegate*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Delegate->ToString<UObject>()));
	return 1;
}

void FLuaFDelegate::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
