#include "LuaTArray.h"
#include "Lua.h"
#include "util/ReflectionHelper.h"
#include "TiReflection.h"
#include <string>
#include "LuaUObject.h"
using namespace std;

using namespace TweakIt;
using namespace Lua;

int LuaTArray::lua_index(lua_State* L) {
	LuaTArray* self = static_cast<LuaTArray*>(lua_touserdata(L, 1));
	int index = lua_tointeger(L, 2)-1;
	LOGFS(FString::Printf(TEXT("Indexing a LuaTArray with %d"), index))
	void* ArrayValue = self->ArrayProperty->ContainerPtrToValuePtr<void>(self->Container);
	FScriptArrayHelper Helper = FScriptArrayHelper(self->ArrayProperty, ArrayValue);
	if(Helper.IsValidIndex(index)) {
		propertyToLua(L, self->ArrayProperty->Inner, Helper.GetRawPtr(index));
	}
	else {
		LOGF("Index %d isn't valid, the array is %d long", index, Helper.Num())
		lua_pushnil(L);
	}
	return 1;
}

int LuaTArray::lua_newindex(lua_State* L) {
	LuaTArray* self = static_cast<LuaTArray*>(lua_touserdata(L, 1));
	int index = lua_tointeger(L, 2)-1;
	LOGF("Newindexing a LuaTArray with %d", index)
	void* ArrayValue = self->ArrayProperty->ContainerPtrToValuePtr<void>(self->Container);
	FScriptArrayHelper Helper = FScriptArrayHelper(self->ArrayProperty, ArrayValue);
	if(Helper.IsValidIndex(index)) {
		luaToProperty(L, self->ArrayProperty->Inner, Helper.GetRawPtr(index), 3);
	}
	else {
		LOGF("Index %d isn't valid, the array is %d long", index, Helper.Num())
        lua_pushnil(L);
	}
	return 1;
}

int LuaTArray::lua__tostring(lua_State* L) {
	LuaTArray* self = static_cast<LuaTArray*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*self->ArrayProperty->GetName()));
	return 1;
}

int LuaTArray::lua_gc(lua_State* L) {
	LuaTArray* self = static_cast<LuaTArray*>(lua_touserdata(L, 1));
	self->~LuaTArray();
	return 0;
}

int LuaTArray::ConstructArray(lua_State* L, UArrayProperty* ArrayProperty, void* Container) {
	if (ArrayProperty && ArrayProperty->IsValidLowLevel()) {
		LOGF("Constructing a LuaTArray from %s", *ArrayProperty->GetName())
		LuaTArray* ReturnedInstance = static_cast<LuaTArray*>(lua_newuserdata(L, sizeof(LuaTArray)));
		new(ReturnedInstance) LuaTArray{ArrayProperty, Container};
		luaL_getmetatable(L, "LuaTArrayMeTa");
		lua_setmetatable(L, -2);
	} else {
		LOG("Trying to construct a LuaTArray from an invalid property")
		lua_pushnil(L);
	}
	return 1;
}
