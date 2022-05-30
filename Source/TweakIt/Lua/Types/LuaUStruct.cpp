#include "LuaUStruct.h"
#include "TweakIt/Helpers/TiReflection.h"
#include <string>
#include "TweakIt/TweakItModule.h"
using namespace std;

int LuaUStruct::lua__index(lua_State* L) {
	LuaUStruct* self = Get(L);
	FString index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUStruct with %s", *index)
	if(lua_CFunction* Method = Methods.Find(index)) {
		lua_pushcfunction(L, *Method);
		return 1;
	}
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(self->Struct, *index);
	if(!NestedProperty->IsValidLowLevel()) {
		LOGF("The struct doesn't have a %s field", *index)
		lua_pushnil(L);
		return 1;
	}
	void* Value = NestedProperty->ContainerPtrToValuePtr<void>(self->Values);
	PropertyToLua(L, NestedProperty, Value);
	return 1;
}

int LuaUStruct::lua__newindex(lua_State* L) {
	LuaUStruct* self = Get(L);
	FString index = luaL_checkstring(L, 2);
	LOGF("Newindexing a LuaUStruct with %s", *index)
	UProperty* NestedProperty = FTIReflection::FindPropertyByName(self->Struct, *index);
	if(!NestedProperty->IsValidLowLevel()) {
		LOGF("The struct doesn't have a %s field", *index)
		lua_pushnil(L);
		return 1;
	}
	void* Value = NestedProperty->ContainerPtrToValuePtr<void>(self->Values);
	LuaToProperty(L, NestedProperty, Value, 3);
	self->Struct->AddToRoot();
	return 1;
}

int LuaUStruct::lua__tostring(lua_State* L) {
	LuaUStruct* self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*self->Struct->GetName()));
	return 1;
}

int LuaUStruct::lua__gc(lua_State* L) {
	LuaUStruct* self = Get(L);
	self->~LuaUStruct();
	return 0;
}

void LuaUStruct::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}

int LuaUStruct::ConstructStruct(lua_State* L, UStruct* Struct, void* Values) {
	if (!Struct->IsValidLowLevel()) {
		LOG("Trying to construct a LuaUStruct from an invalid property")
		lua_pushnil(L);
		return 1;
	}
	
	LOGF("Constructing a LuaUStruct from %s", *Struct->GetName())
	LuaUStruct* ReturnedInstance = static_cast<LuaUStruct*>(lua_newuserdata(L, sizeof(LuaUStruct)));
	new(ReturnedInstance) LuaUStruct{Struct, Values};
	luaL_getmetatable(L, LuaUStruct::Name);
	lua_setmetatable(L, -2);
	return 1;
}

LuaUStruct* LuaUStruct::Get(lua_State* L, int i){
	return static_cast<LuaUStruct*>(luaL_checkudata(L, i, Name));
}