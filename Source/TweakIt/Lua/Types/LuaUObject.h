#pragma once
#include "CoreMinimal.h"

#include "TweakIt/Lua/Lua.h"

struct FLuaUObject
{
	explicit FLuaUObject(UObject* Object);

	UObject* Object;

	static int ConstructObject(lua_State* L, UObject* Object);
	static FLuaUObject* Get(lua_State* L, int Index = 1);

	void CaptureObject();
	void ReleaseObject();

	static int Lua_DumpProperties(lua_State* L);
	static int Lua_GetClass(lua_State* L);

	static int Lua__index(lua_State* L);
	static int Lua__newindex(lua_State* L);
	static int Lua__tostring(lua_State* L);
	static int Lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "UObject";

private:
	inline static TMap<FString, lua_CFunction> Methods = {
		{"GetClass", Lua_GetClass},
		{"DumpProperties", Lua_DumpProperties},
	};

	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__newindex", Lua__newindex},
		{"__tostring", Lua__tostring},
		{"__gc", Lua__gc},
	};
	bool MadeRoot = false;
};
