#pragma once
#include "CoreMinimal.h"

#include "TweakIt/Lua/Lua.h"

struct FLuaUClass
{
	UClass* Class;

	static int ConstructClass(lua_State* L, UClass* Class);
	static FLuaUClass* Get(lua_State* L, int Index = 1);

	static int Lua_GetDefaultValue(lua_State* L);
	static int Lua_ChangeDefaultValue(lua_State* L);
	static int Lua_AddDefaultComponent(lua_State* L);
	static int Lua_RemoveDefaultComponent(lua_State* L);
	static int Lua_GetChildClasses(lua_State* L);
	static int Lua_GetObjects(lua_State* L);
	static int Lua_DumpProperties(lua_State* L);
	static int Lua_MakeSubclass(lua_State* L);

	static int Lua__index(lua_State* L);
	static int Lua__newindex(lua_State* L);
	static int Lua__call(lua_State* L);
	static int Lua__tostring(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "UClass";

private:
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__newindex", Lua__newindex},
		{"__call", Lua__call},
		{"__tostring", Lua__tostring},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		{"GetDefaultValue", Lua_GetDefaultValue},
		{"ChangeDefaultValue", Lua_ChangeDefaultValue},
		{"GetChildClasses", Lua_GetChildClasses},
		{"GetObjects", Lua_GetObjects},
		{"MakeSubclass", Lua_MakeSubclass},
		{"AddDefaultComponent", Lua_AddDefaultComponent},
		{"RemoveDefaultComponent", Lua_RemoveDefaultComponent},
		{"DumpProperties", Lua_DumpProperties},
	};
};
