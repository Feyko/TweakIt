#include "LuaUStruct.h"
#include "TweakIt/Helpers/TiReflection.h"
#include <string>
#include "TweakIt/Logging/FTILog.h"
using namespace std;

FLuaUStruct::FLuaUStruct(UStruct* Struct, void* Values) : Struct(Struct), Values(Values)
{
	
}

int FLuaUStruct::ConstructStruct(lua_State* L, UStruct* Struct, void* Values, bool Owning)
{
	if (!Struct->IsValidLowLevel())
	{
		LOG("Trying to construct a LuaUStruct from an invalid property")
		lua_pushnil(L);
		return 1;
	}
	LOGF("Constructing a LuaUStruct from %s", *Struct->GetName())
	FLuaUStruct** ReturnedInstance = static_cast<FLuaUStruct**>(lua_newuserdata(L, sizeof(FLuaUStruct*)));
	*ReturnedInstance = new FLuaUStruct(Struct, Values);
	luaL_getmetatable(L, Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUStruct* FLuaUStruct::Get(lua_State* L, int Index)
{
	return *static_cast<FLuaUStruct**>(luaL_checkudata(L, Index, Name));
}

void FLuaUStruct::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Struct);
}

int FLuaUStruct::Lua_Copy(lua_State* L)
{
	FLuaUStruct* Self = Get(L);
	void* Copy = FTIReflection::CopyStruct(Self->Struct, Self->Values);
	return ConstructStruct(L, Self->Struct, Copy);
}

int FLuaUStruct::Lua_MakeStructInstance(lua_State* L)
{
	return FTILua::Lua_MakeStructInstance(L);
}

int FLuaUStruct::Lua__index(lua_State* L)
{
	FLuaUStruct* Self = Get(L);
	FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUStruct with %s", *Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(Self->Struct, *Index);
	if (!NestedProperty->IsValidLowLevel())
	{
		LOGF("The struct doesn't have a %s field", *Index)
		lua_pushnil(L);
		return 1;
	}
	FTILua::PropertyToLua(L, NestedProperty, Self->Values);
	return 1;
}

int FLuaUStruct::Lua__newindex(lua_State* L)
{
	FLuaUStruct* Self = Get(L);
	FString Index = luaL_checkstring(L, 2);
	LOGF("Newindexing a LuaUStruct with %s", *Index)
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(Self->Struct, *Index);
	if (!NestedProperty->IsValidLowLevel())
	{
		LOGF("The struct doesn't have a %s field", *Index)
		lua_pushnil(L);
		return 1;
	}
	FTILua::LuaToProperty(L, NestedProperty, Self->Values, 3);
	return 1;
}

int FLuaUStruct::Lua__tostring(lua_State* L)
{
	FLuaUStruct* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Struct->GetName()));
	return 1;
}

int FLuaUStruct::Lua__gc(lua_State* L)
{
	FLuaUStruct* Self = Get(L);
	delete Self;
	return 0;
}

void FLuaUStruct::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
