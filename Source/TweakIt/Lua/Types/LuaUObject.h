#pragma once
#include "CoreMinimal.h"

#include "TweakIt\Lua\Lua.h"

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
	static LuaUObject* Get(lua_State* L, int i = 1);
	
	inline static const char* Name = "UObject";
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", lua__index},
		{"__newindex", lua__newindex},
		{"__tostring", lua__tostring},
		{"__gc", lua__gc},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		// {"GetClass", lua_GetClass},
		{"DumpProperties", lua_DumpProperties},
	};
};