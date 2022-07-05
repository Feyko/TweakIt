#include "LuaFDelegate.h"
#include <string>

#include "TweakIt/TweakItTesting.h"
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
	LOG("LuaFDelegate::Bind")
    FLuaFDelegate* Self = Get(L);
	luaT_CheckLuaFunction(L, 2);
	FString FunctionName = UTIUFunctionBinder::MakeFunctionName(FTILog::CurrentScript, Self->SignatureFunction->GetName());
	LOG("Dumping")
	FTILuaFuncManager::DumpFunction(L, FunctionName, 2);
	UFunction* Function = nullptr;
	UE4CodeGen_Private::FFunctionParams Params = UE4CodeGen_Private::FFunctionParams();
	Params.OwningClassName = TCHAR_TO_UTF8(*UTIUFunctionBinder::StaticClass()->GetName());
	Params.NameUTF8 = TCHAR_TO_UTF8(*FunctionName);
	Params.OuterFunc = []()->UObject*{return UTIUFunctionBinder::StaticClass();};
	Params.FunctionFlags = FUNC_Native|FUNC_Static|FUNC_Public;
	LOG("Constructiong")
	ConstructUFunction(Function, Params);
	LOG("Making and setting")
	Function->SetNativeFunc(FTILuaFuncManager::SavedLuaFuncToNativeFunc(L, FunctionName));
	LOG("Adding")
	UTIUFunctionBinder::AddFunction(Function, FunctionName);
	LOG("Binding")
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

	return 0;
}

int FLuaFDelegate::Lua_Trigger(lua_State* L)
{
	luaL_error(L, "Trigger is WIP");
	return 0;
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
	RegisterMetatable(L, Name, Metadata);
}
