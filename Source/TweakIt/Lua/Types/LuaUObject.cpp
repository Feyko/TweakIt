#include "LuaUObject.h"

#include "LuaUClass.h"
#include <string>

#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Helpers/TIReflection.h"
using namespace std;

FLuaUObject::FLuaUObject(UObject* Object) : Object(Object)
{
	
}

int FLuaUObject::ConstructObject(lua_State* L, UObject* Object)
{
	LOG("Constructing a LuaUObject")
	if (!Object->IsValidLowLevel())
	{
		LOG("Trying to construct a LuaUObject from an invalid object")
		lua_pushnil(L);
		return 1;
	}
	FLuaUObject** ReturnedInstance = static_cast<FLuaUObject**>(lua_newuserdata(L, sizeof(FLuaUObject*)));
	*ReturnedInstance = new FLuaUObject(Object);
	luaL_getmetatable(L, FLuaUObject::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUObject* FLuaUObject::Get(lua_State* L, int Index)
{
	return *static_cast<FLuaUObject**>(luaL_checkudata(L, Index, Name));
}

void FLuaUObject::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Object);
}

int FLuaUObject::Lua_DumpProperties(lua_State* L)
{
	FLuaUObject* Self = Get(L);
	LOG("Dumping the properties for " + Self->Object->GetName())
	if (Self->Object->IsA(AActor::StaticClass()))
	{
		AActor* Actor = Cast<AActor>(Self->Object);
		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		for (auto Component : Components)
		{
			LOG(Component->GetName())
		}
	}
	for (UProperty* Property = Self->Object->GetClass()->PropertyLink; Property; Property = Property->PropertyLinkNext
	)
	{
		LOG(Property->GetName())
	}
	return 0;
}

int FLuaUObject::Lua_GetClass(lua_State* L)
{
	FLuaUObject* Self = Get(L);
	FLuaUClass::ConstructClass(L, Self->Object->GetClass());
	return 1;
}

int FLuaUObject::Lua__index(lua_State* L)
{
	FLuaUObject* Self = Get(L);
	FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUObject with %s", *Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	UProperty* Property = FTIReflection::FindPropertyByName(Self->Object->GetClass(), *Index);
	if (!Property->IsValidLowLevel())
	{
		UFunction* Function = FTIReflection::FindFunctionByName(Self->Object->GetClass(), *Index);
		if (!Function)
		{
			lua_pushnil(L);
			return 1;
		}
		FTILua::UFunctionToLua(L, Function, Self->Object);
		return 1;
	}
	LOG("Found property")
	FTILua::PropertyToLua(L, Property, Self->Object);
	return 1;
}

int FLuaUObject::Lua__newindex(lua_State* L)
{
	{
		FLuaUObject* Self = Get(L);
		FString PropertyName = luaL_checkstring(L, 2);
		LOGF("Newindexing a LuaUObject with %s", *PropertyName)
		UProperty* Property = FTIReflection::FindPropertyByName(Self->Object->GetClass(), *PropertyName);
		if (!Property->IsValidLowLevel())
		{
			LOGF("No property '%s' found", *PropertyName)
			return 0;
		}
		LOG("Found property")
		FTILua::LuaToProperty(L, Property, Self->Object, 3);
		return 0;
	}
}

int FLuaUObject::Lua__tostring(lua_State* L)
{
	FLuaUObject* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Object->GetName()));
	return 1;
}

int FLuaUObject::Lua__gc(lua_State* L)
{
	FLuaUObject* Self = Get(L);
	delete Self;
	return 0;
}

void FLuaUObject::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
