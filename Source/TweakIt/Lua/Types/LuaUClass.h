#pragma once
#include "CoreMinimal.h"

#include "TweakIt\Lua\Lua.h"

int lua_MakeSubclass(lua_State* L); // Forward declaration. I hate C++

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
	static LuaUClass* Get(lua_State* L, int i = 1);

	inline static const char* Name = "UClass";
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", lua__index},
		{"__newindex", lua__newindex},
		{"__call", lua__call},
		{"__tostring", lua__tostring},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		{"GetDefaultValue", lua_GetDefaultValue},
		{"ChangeDefaultValue", lua_ChangeDefaultValue},
		{"GetChildClasses", lua_GetChildClasses},
		{"GetObjects", lua_GetObjects},
		{"MakeSubclass", lua_MakeSubclass},
		{"AddDefaultComponent", lua_AddDefaultComponent},
		{"RemoveDefaultComponent", lua_RemoveDefaultComponent},
		{"DumpProperties", lua_DumpProperties},
	};
};