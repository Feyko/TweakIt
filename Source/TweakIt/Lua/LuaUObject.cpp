#include "LuaUObject.h"

#include "LuaUClass.h"
#include "Lua.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include <string>

#include "TweakIt/TweakItModule.h"
#include "TweakIt/Helpers/TIReflection.h"
using namespace std;

int LuaUObject::lua__index(lua_State* L) {

	LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
	FString PropertyName = lua_tostring(L, 2);
	LOGF("Indexing a LuaUObject with %s",*PropertyName)
	if (PropertyName == "GetClass") {
		lua_pushcfunction(L, lua_GetClass);
	} else if (PropertyName == "DumpProperties") {
		lua_pushcfunction(L, lua_DumpProperties);
	}
	UProperty* Property = FTIReflection::FindPropertyByName(self->Object->GetClass(), *PropertyName);
	if (Property->IsValidLowLevel()) {
		LOG("Found property")
		PropertyToLua(L, Property, self->Object);
	} else { lua_pushnil(L); }
	return 1;
}

int LuaUObject::lua__newindex(lua_State* L) {
	{
		LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
		FString PropertyName = lua_tostring(L, 2);
		LOGF("Newindexing a LuaUObject with %s",*PropertyName)
		UProperty* Property = FTIReflection::FindPropertyByName(self->Object->GetClass(), *PropertyName);
		if (Property->IsValidLowLevel()) {
			LOG("Found property")
			LuaToProperty(L, Property, self->Object, 3);
		} else { lua_pushnil(L); }
		return 0;
	}
}

int LuaUObject::lua_DumpProperties(lua_State* L) {
	LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
	LOGFS(FString("Dumping the properties for " + self->Object->GetName()))
	if (self->Object->IsA(AActor::StaticClass())) {
		AActor* Actor = Cast<AActor>(self->Object);
		TArray<UActorComponent*> components;
		Actor->GetComponents(components);
		for (auto component : components) {
			LOGFS(component->GetName())
		}
	}
	for (UProperty* Property = self->Object->GetClass()->PropertyLink; Property; Property = Property->PropertyLinkNext
	) {
		LOGFS(Property->GetName())
	}
	return 0;
}

int LuaUObject::ConstructObject(lua_State* L, UObject* Object) {
	LOG("Constructing a LuaUObject")
	if (Object->IsValidLowLevel()) {
		LOG("Actualling constructing")
		LuaUObject* ReturnedInstance = static_cast<LuaUObject*>(lua_newuserdata(L, sizeof(LuaUObject)));
		LOG("Got instance")
		new(ReturnedInstance) LuaUObject{Object};
		LOG("Initialised")
		luaL_getmetatable(L, "UObjectMeta");
		lua_setmetatable(L, -2);
	} else {
		LOG("[TweakIt] Trying to construct a LuaUObject from an invalid object")
		lua_pushnil(L);
	}
	return 1;
}

int LuaUObject::lua__gc(lua_State* L) {
	LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
	self->~LuaUObject();
	return 0;
}

int LuaUObject::lua_GetClass(lua_State* L) {
	LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
	LuaUClass::ConstructClass(L, self->Object->GetClass());
	return 1;
}

int LuaUObject::lua__tostring(lua_State* L) {
	LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
	lua_pushstring(L, TCHAR_TO_UTF8(*self->Object->GetName()));
	return 1;
}

void LuaUObject::RegisterMetadata(lua_State* L)
{
	luaL_newmetatable(L, "UObjectMeta");
	RegisterMethod(L, "__index", lua__index);
	RegisterMethod(L, "__newindex", lua__newindex);
	RegisterMethod(L, "__tostring", lua__tostring);
	RegisterMethod(L, "__gc", lua__gc);
}
