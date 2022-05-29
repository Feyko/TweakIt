#include "LuaUStruct.h"
#include "Lua.h"
#include "TweakIt/Helpers/TiReflection.h"
#include <string>
#include "TweakIt/Lua/lib/lua.hpp"
#include "TweakIt/TweakItModule.h"
using namespace std;

int LuaUStruct::lua__index(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	FString index = lua_tostring(L, 2);
	LOGF("Indexing a LuaUStruct with %s", *index)
	if(index == "MakeStructInstance") {
		lua_pushcfunction(L, lua_MakeStructInstance);
		return 1;
	}
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(self->Struct, *index);
	if(NestedProperty->IsValidLowLevel()) {
		void* Value = NestedProperty->ContainerPtrToValuePtr<void>(self->Values);
		PropertyToLua(L, NestedProperty, Value);
	}
	else {
		LOGF("The struct doesn't have a %s field", *index)
		lua_pushnil(L);
	}
	return 1;
}

int LuaUStruct::lua__newindex(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	FString index = lua_tostring(L, 2);
	LOGF("Newindexing a LuaUStruct with %s", *index)
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(self->Struct, *index);
	if(NestedProperty->IsValidLowLevel()) {
		void* Value = NestedProperty->ContainerPtrToValuePtr<void>(self->Values);
		LuaToProperty(L, NestedProperty, Value, 3);
		self->Struct->AddToRoot();
	}
	else {
		LOGF("The struct doesn't have a %s field", *index)
        lua_pushnil(L);
	}
	return 1;
}

int LuaUStruct::lua__tostring(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*self->Struct->GetName()));
	return 1;
}

int LuaUStruct::lua__gc(lua_State* L) {
	LuaUStruct* self = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
	self->~LuaUStruct();
	return 0;
}

void LuaUStruct::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}

int LuaUStruct::ConstructStruct(lua_State* L, UStruct* Struct, void* Values) {
	if (Struct->IsValidLowLevel()) {
		LOGF("Constructing a LuaUStruct from %s", *Struct->GetName())
		LuaUStruct* ReturnedInstance = static_cast<LuaUStruct*>(lua_newuserdata(L, sizeof(LuaUStruct)));
		new(ReturnedInstance) LuaUStruct{Struct, Values};
		luaL_getmetatable(L, LuaUStruct::Name);
		lua_setmetatable(L, -2);
	} else {
		LOG("Trying to construct a LuaUStruct from an invalid property")
		lua_pushnil(L);
	}
	return 1;
}
