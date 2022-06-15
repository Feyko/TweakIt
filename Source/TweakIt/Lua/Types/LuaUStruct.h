#pragma once
#include "CoreMinimal.h"

#include "TweakIt/Lua/Lua.h"

int Lua_MakeStructInstance(lua_State* L); // Forward declaration. I hate C++

struct FLuaUStruct
{
	UStruct* Struct;
	void* Values;
	bool Owning;

	static int ConstructStruct(lua_State* L, UStruct* Struct, void* Values, bool Owning = false);
	static FLuaUStruct* Get(lua_State* L, int Index = 1);

	static int Lua_Copy(lua_State* L);

	static int Lua__index(lua_State* L);
	static int Lua__newindex(lua_State* L);
	static int Lua__tostring(lua_State* L);
	static int Lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "UStruct";

private:
	inline static TMap<FString, lua_CFunction> Methods = {
		{"MakeStructInstance", Lua_MakeStructInstance},
		{"Copy", Lua_Copy}
	};

	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__newindex", Lua__newindex},
		{"__tostring", Lua__tostring},
		{"__gc", Lua__gc},
	};
};
