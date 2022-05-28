#pragma once

#include "TweakIt\Lua\lib\lua.hpp"

struct LuaTArray
{
	UArrayProperty* ArrayProperty;
	void* Container;

	static int lua__index(lua_State* L);

	static int lua__newindex(lua_State* L);

	static int lua__tostring(lua_State* L);
	static int lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	static int ConstructArray(lua_State* L, UArrayProperty* ArrayProperty, void* Container);
};