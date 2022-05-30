#pragma once
#include "CoreMinimal.h"

#include "TweakIt\Lua\Lua.h"

struct LuaUClass
{
	UClass* Class;

	static int lua_GetDefaultValue(lua_State* L);
	static int lua__index(lua_State* L);
	static int lua_ChangeDefaultValue(lua_State* L);
	static int lua_AddDefaultComponent(lua_State* L);
	static int lua_RemoveDefaultComponent(lua_State* L);
	static int lua_GetChildClasses(lua_State* L);
	static int lua_GetObjects(lua_State* L);
	static int lua_DumpProperties(lua_State* L);

	static int lua__newindex(lua_State* L);

	static int lua__call(lua_State* L);
	static int lua__tostring(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	static int ConstructClass(lua_State* L, UClass* Class);

	inline static const char* Name = "UClass";
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", lua__index},
		{"__newindex", lua__newindex},
		{"__call", lua__call},
		{"__tostring", lua__tostring},
	};
};