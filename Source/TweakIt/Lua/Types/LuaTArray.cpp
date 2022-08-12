#include "LuaTArray.h"
#include "TweakIt/Lua/Lua.h"
#include <string>

#include "TweakIt/Logging/FTILog.h"
using namespace std;

FLuaTArray::FLuaTArray(FArrayProperty* Property, void* Container) : ArrayProperty(Property), Container(Container)
{
	
}

int FLuaTArray::ConstructArray(lua_State* L, FArrayProperty* ArrayProperty, void* Container)
{
	if (!ArrayProperty->IsValidLowLevel())
	{
		LOG("Trying to construct a LuaTArray from an invalid property")
		lua_pushnil(L);
		return 1;
	}
	LOGF("Constructing a LuaTArray from %s", *ArrayProperty->GetName())
	FLuaTArray** ReturnedInstance = static_cast<FLuaTArray**>(lua_newuserdata(L, sizeof(FLuaTArray*)));
	*ReturnedInstance = new FLuaTArray(ArrayProperty, Container);
	luaL_getmetatable(L, FLuaTArray::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaTArray* FLuaTArray::Get(lua_State* L, int Index)
{
	return *static_cast<FLuaTArray**>(luaL_checkudata(L, Index, Name));
}

void FLuaTArray::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(ArrayProperty);
}

int FLuaTArray::Lua__index(lua_State* L)
{
	FLuaTArray* Self = Get(L);
	int Index = luaL_checkinteger(L, 2) - 1;
	LOGF("Indexing a LuaTArray with %d", Index)
	FScriptArray* ArrayValue = Self->ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(Self->Container);
	if (!ArrayValue->IsValidIndex(Index))
	{
		LOGF("Index %d isn't valid, the array is %d long", Index + 1, ArrayValue->Num())
		lua_pushnil(L);
		return 1;
	}
	FTILua::PropertyToLua(L, Self->ArrayProperty->Inner,
	              static_cast<uint8*>(ArrayValue->GetData()) + Self->ArrayProperty->Inner->ElementSize * Index);
	return 1;
}

int FLuaTArray::Lua__newindex(lua_State* L)
{
	FLuaTArray* Self = Get(L);
	int Index = luaL_checkinteger(L, 2) - 1;
	LOGF("Newindexing a LuaTArray with %d", Index)
	FScriptArray* ArrayValue = Self->ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(Self->Container);
	if (!ArrayValue->IsValidIndex(Index))
	{
		int appendCount = (Index + 1) - ArrayValue->Num();
		LOGF("Creating %d values", appendCount)
		ArrayValue->Add(appendCount, Self->ArrayProperty->Inner->ElementSize);
	}
	FTILua::LuaToProperty(L, Self->ArrayProperty->Inner,
	              static_cast<uint8*>(ArrayValue->GetData()) + Self->ArrayProperty->Inner->ElementSize * Index, 3);
	return 0;
}

int FLuaTArray::Lua__tostring(lua_State* L)
{
	FLuaTArray* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->ArrayProperty->GetName()));
	return 1;
}

int FLuaTArray::Lua__len(lua_State* L)
{
	FLuaTArray* Self = Get(L);
	FScriptArray* Value = Self->ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(Self->Container);
	lua_pushinteger(L, Value->Num());
	return 1;
}

int FLuaTArray::Lua__gc(lua_State* L)
{
	FLuaTArray* Self = Get(L);
	delete Self;
	return 0;
}

void FLuaTArray::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
