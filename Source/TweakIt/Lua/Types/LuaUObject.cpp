#include "LuaUObject.h"

#include "LuaUClass.h"
#include <string>

#include "TweakIt/TweakItModule.h"
#include "TweakIt/Helpers/TIReflection.h"
using namespace std;

int FLuaUObject::ConstructObject(lua_State* L, UObject* Object) {
	LOG("Constructing a LuaUObject")
	if (!Object->IsValidLowLevel()) {
		LOG("Trying to construct a LuaUObject from an invalid object")
		lua_pushnil(L);
		return 1;
	}
	Object->AddToRoot();
	FLuaUObject* ReturnedInstance = static_cast<FLuaUObject*>(lua_newuserdata(L, sizeof(FLuaUObject)));
	new(ReturnedInstance) FLuaUObject{Object};
	luaL_getmetatable(L, FLuaUObject::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUObject* FLuaUObject::Get(lua_State* L, int Index)
{
	return static_cast<FLuaUObject*>(luaL_checkudata(L, Index, Name));
}

int FLuaUObject::Lua_DumpProperties(lua_State* L) {
	FLuaUObject* Self = Get(L);
	LOGFS(FString("Dumping the properties for " + Self->Object->GetName()))
	if (Self->Object->IsA(AActor::StaticClass())) {
		AActor* Actor = Cast<AActor>(Self->Object);
		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		for (auto Component : Components) {
			LOGFS(Component->GetName())
		}
	}
	for (UProperty* Property = Self->Object->GetClass()->PropertyLink; Property; Property = Property->PropertyLinkNext
	) {
		LOGFS(Property->GetName())
	}
	return 0;
}

int FLuaUObject::Lua_GetClass(lua_State* L) {
	FLuaUObject* Self = Get(L);
	FLuaUClass::ConstructClass(L, Self->Object->GetClass());
	return 1;
}

int FLuaUObject::Lua__index(lua_State* L) {
	FLuaUObject* Self = Get(L);
	FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUObject with %s",*Index)
	if(lua_CFunction* Method = Methods.Find(Index)) {
		lua_pushcfunction(L, *Method);
		return 1;
	}
	UProperty* Property = FTIReflection::FindPropertyByName(Self->Object->GetClass(), *Index);
	if (!Property->IsValidLowLevel()) {
		lua_pushnil(L);
		return 1; 
	}
	LOG("Found property")
	PropertyToLua(L, Property, Self->Object);
	return 1;
}

int FLuaUObject::Lua__newindex(lua_State* L) {
	{
		FLuaUObject* Self = Get(L);
		FString PropertyName = luaL_checkstring(L, 2);
		LOGF("Newindexing a LuaUObject with %s",*PropertyName)
		UProperty* Property = FTIReflection::FindPropertyByName(Self->Object->GetClass(), *PropertyName);
		if (!Property->IsValidLowLevel()) {
			LOGF("No property '%s' found", *PropertyName)
			return 0;
		}
		LOG("Found property")
		LuaToProperty(L, Property, Self->Object, 3);
		return 0;
	}
}

int FLuaUObject::Lua__tostring(lua_State* L) {
	FLuaUObject* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Object->GetName()));
	return 1;
}

int FLuaUObject::Lua__gc(lua_State* L) {
	FLuaUObject* Self = Get(L);
	Self->Object->RemoveFromRoot();
	return 0;
}

void FLuaUObject::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}