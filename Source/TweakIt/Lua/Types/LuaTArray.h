#pragma once

#include "TweakIt\Lua\lib\lua.hpp"

struct LuaTArray
{
	UArrayProperty* ArrayProperty;
	void* Container;

	static int lua__index(lua_State* L);

	static int lua__newindex(lua_State* L);

	static int lua__tostring(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	static int ConstructArray(lua_State* L, UArrayProperty* ArrayProperty, void* Container);
	static LuaTArray* Get(lua_State* L, int i = 1);

	inline static const char* Name = "TArray";
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", lua__index},
		{"__newindex", lua__newindex},
		{"__tostring", lua__tostring},
	};
};