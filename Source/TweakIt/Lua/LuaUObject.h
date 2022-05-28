#pragma once
#include "CoreMinimal.h"

#include <string>

#include "TweakIt\Lua\lib\lua.hpp"

struct LuaUObject
{
	UObject* Object;

	static int lua__index(lua_State* L);

	static int lua__newindex(lua_State* L);
	static int lua_DumpProperties(lua_State* L);
	static int ConstructObject(lua_State* L, UObject* Object);
	static int lua__gc(lua_State* L);
	static int lua_GetClass(lua_State* L);
	static int lua__tostring(lua_State* L);

	static void RegisterMetadata(lua_State* L);
};