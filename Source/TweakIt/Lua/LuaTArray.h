#pragma once

#include "TweakIt\Lua\lib\lua.hpp"

struct LuaTArray
{
	UArrayProperty* ArrayProperty;
	void* Container;

	static int lua_index(lua_State* L);

	static int lua_newindex(lua_State* L);

	static int lua__tostring(lua_State* L);
	static int lua_gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	static int ConstructArray(lua_State* L, UArrayProperty* ArrayProperty, void* Container);
};