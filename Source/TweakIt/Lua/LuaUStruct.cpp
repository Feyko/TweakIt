#include "LuaUStruct.h"
#include "Lua.h"
#include "util/ReflectionHelper.h"
#include "TiReflection.h"
#include <string>
#include "LuaUObject.h"
using namespace std;

using namespace TweakIt;
using namespace Lua;

int LuaUStruct::lua_index(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	FString index = lua_tostring(L, 2);
	LOGFS(FString::Printf(TEXT("Indexing a LuaUStruct with %s"), *index))
	void* ScriptValue = self->ScriptProperty->ContainerPtrToValuePtr<void>(self->Container);
	UScriptStruct* ScriptStruct = self->ScriptProperty->Struct;
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(ScriptStruct, *index);
	if(NestedProperty->IsValidLowLevel()) {
		propertyToLua(L, NestedProperty, ScriptValue);
	}
	else {
		LOGF("The struct doesn't have a %s field", *index)
		lua_pushnil(L);
	}
	return 1;
}

int LuaUStruct::lua_newindex(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	FString index = lua_tostring(L, 2);
	LOGFS(FString::Printf(TEXT("Newindexing a LuaUStruct with %s"), *index))
    void* ScriptValue = self->ScriptProperty->ContainerPtrToValuePtr<void>(self->Container);
	UScriptStruct* ScriptStruct = self->ScriptProperty->Struct;
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(ScriptStruct, *index);
	if(NestedProperty->IsValidLowLevel()) {
		luaToProperty(L, NestedProperty, ScriptValue, 3);
	}
	else {
		LOGF("The struct doesn't have a %s field", *index)
        lua_pushnil(L);
	}
	return 1;
}

int LuaUStruct::lua__tostring(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*self->ScriptProperty->GetName()));
	return 1;
}

int LuaUStruct::lua_gc(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	self->~LuaUStruct();
	return 0;
}

int LuaUStruct::ConstructStruct(lua_State* L, UStructProperty* StructProperty, void* Container) {
	if (StructProperty && StructProperty->IsValidLowLevel()) {
		LOGF("Constructing a LuaUStruct from %s", *StructProperty->GetName())
		LuaUStruct* ReturnedInstance = static_cast<LuaUStruct*>(lua_newuserdata(L, sizeof(LuaUStruct)));
		new(ReturnedInstance) LuaUStruct{StructProperty, Container};
		luaL_getmetatable(L, "LuaUStructMeTa");
		lua_setmetatable(L, -2);
	} else {
		LOG("Trying to construct a LuaUStruct from an invalid property")
		lua_pushnil(L);
	}
	return 1;
}
