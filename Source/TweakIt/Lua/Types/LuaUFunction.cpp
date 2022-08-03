#include "LuaUFunction.h"

#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/FTILuaFuncManager.h"

FLuaUFunction::FLuaUFunction(UFunction* Function, UObject* Object) : Function(Function), Object(Object)
{
	
}

void FLuaUFunction::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Object);
	Collector.AddReferencedObject(Function);
}

int FLuaUFunction::Construct(lua_State* L, UFunction* Function, UObject* Object)
{
	LOG("Constructing a LuaUFunction")
	if (!Function->IsValidLowLevel())
	{
		LOG("Trying to construct a LuaUFunction from an invalid function")
		lua_pushnil(L);
		return 1;
	}
	FLuaUFunction** ReturnedInstance = static_cast<FLuaUFunction**>(lua_newuserdata(L, sizeof(FLuaUFunction*)));
	*ReturnedInstance = new FLuaUFunction(Function, Object);
	luaL_getmetatable(L, FLuaUFunction::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUFunction* FLuaUFunction::Get(lua_State* L, int Index)
{
	return *static_cast<FLuaUFunction**>(luaL_checkudata(L, Index, Name));
}

int FLuaUFunction::Lua_On(lua_State* L)
{
	FLuaUFunction* Self = Get(L);
	UObject* Object = FLuaUObject::Get(L, 2)->Object;
	UFunction* Function = Object->FindFunction(FName(Self->Function->GetName()));
	if (Self->Function != Function)
	{
		luaL_error(L, TCHAR_TO_UTF8(
			*FString::Printf(TEXT("Tried to assign object %s to function %s but its class does not have the function"),
			*Object->GetName(), *Function->GetName())));
		return 0;
	}
	lua_settop(L, 1); // We set the top to 1 and return 1 to return Self
	return 1;
}

int FLuaUFunction::Lua_Bind(lua_State* L)
{
	FLuaUFunction* Self = Get(L);
	FTILua::LuaT_ExpectLuaFunction(L, 2);
	FString FunctionName = Self->Function->GetFullName();
	LOG(FunctionName)
	FTILuaFuncManager::DumpFunction(L, FunctionName, 2);
	LOG(FTILuaFuncManager::GetSavedLuaFunc(FunctionName).IsOk())
	FNativeFuncPtr Func = FTILuaFuncManager::SavedLuaFuncToNativeFunc(L, FunctionName);
	Self->Function->SetNativeFunc(Func);
	return 0;
}

int FLuaUFunction::Lua__index(lua_State* L)
{
	FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUFunction with %s", *Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

int FLuaUFunction::Lua__call(lua_State* L)
{
	FLuaUFunction* Self = Get(L);
	return FTILua::CallUFunction(L, Self->Object, Self->Function, 2);
}

int FLuaUFunction::Lua__tostring(lua_State* L)
{
	FLuaUFunction* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Function->GetName()));
	return 1;
}

int FLuaUFunction::Lua__gc(lua_State* L)
{
	FLuaUFunction* Self = Get(L);
	delete Self;
	return 0;
}

void FLuaUFunction::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
