#include "LuaTArray.h"
#include "TweakIt/Lua/Lua.h"
#include <string>

#include "TweakIt/TweakItModule.h"
using namespace std;

int LuaTArray::lua__index(lua_State* L) {
	LuaTArray* self = Get(L);
	int index = luaL_checkinteger(L, 2)-1;
	LOGF("Indexing a LuaTArray with %d", index)
	FScriptArray* ArrayValue = self->ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(self->Container);
	if(!ArrayValue->IsValidIndex(index)) {
		LOGF("Index %d isn't valid, the array is %d long", index+1, ArrayValue->Num())
		lua_pushnil(L);
		return 1;
	}
	PropertyToLua(L, self->ArrayProperty->Inner, (uint8*)ArrayValue->GetData() + self->ArrayProperty->Inner->ElementSize * index);
	return 1;
}

int LuaTArray::lua__newindex(lua_State* L) {
	LuaTArray* self = Get(L);
	int index = luaL_checkinteger(L, 2)-1;
	LOGF("Newindexing a LuaTArray with %d", index)
	FScriptArray* ArrayValue = self->ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(self->Container);
	if(!ArrayValue->IsValidIndex(index)) {
		int appendCount = (index + 1) - ArrayValue->Num();
		LOGF("Creating %d values", appendCount)
		ArrayValue->Add(appendCount, self->ArrayProperty->Inner->ElementSize);
	}
	LuaToProperty(L, self->ArrayProperty->Inner, (uint8*)ArrayValue->GetData() + self->ArrayProperty->Inner->ElementSize * index, 3);
	return 0;
}

int LuaTArray::lua__tostring(lua_State* L) {
	LuaTArray* self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*self->ArrayProperty->GetName()));
	return 1;
}

void LuaTArray::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}

int LuaTArray::ConstructArray(lua_State* L, UArrayProperty* ArrayProperty, void* Container) {
	if (!ArrayProperty->IsValidLowLevel()) {
		LOG("Trying to construct a LuaTArray from an invalid property")
		lua_pushnil(L);
		return 1;
	} 
	LOGF("Constructing a LuaTArray from %s", *ArrayProperty->GetName())
	LuaTArray* ReturnedInstance = static_cast<LuaTArray*>(lua_newuserdata(L, sizeof(LuaTArray)));
	new(ReturnedInstance) LuaTArray{ArrayProperty, Container};
	luaL_getmetatable(L, LuaTArray::Name);
	lua_setmetatable(L, -2);
	return 1;
}

LuaTArray* LuaTArray::Get(lua_State* L, int i){
	return static_cast<LuaTArray*>(luaL_checkudata(L, i, Name));
}