#include "LuaUFunction.h"

#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/FTILuaFuncManager.h"

FLuaUFunction::FLuaUFunction(UFunction* Function, UObject* Object) : Function(Function), Object(Object)
{
	
}

FLuaUFunction::~FLuaUFunction()
{
	LOG("DESTROYING FUNCTION")
}

void FLuaUFunction::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Object);
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
	FLuaUFunction* ReturnedInstance = static_cast<FLuaUFunction*>(lua_newuserdata(L, sizeof(FLuaUFunction)));
	new(ReturnedInstance) FLuaUFunction(Function, Object);
	luaL_getmetatable(L, FLuaUFunction::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUFunction* FLuaUFunction::Get(lua_State* L, int Index)
{
	return static_cast<FLuaUFunction*>(luaL_checkudata(L, Index, Name));
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

void FLuaUFunction::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
