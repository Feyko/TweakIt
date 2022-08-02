#pragma once

#include "CoreMinimal.h"
#include "lib/lua.hpp"
#include "lib/lua.h"
#include "Types/include.h"

class FTILua
{
public:
	static void LuaT_ExpectLuaFunction(lua_State* L, int Index);
	template<typename T>
	static T* LuaT_CheckLightUserdata(lua_State* L, int Index);
	static bool LuaT_CheckBoolean(lua_State* L, int Index);
	static bool LuaT_OptBoolean(lua_State* L, int Index, bool Default);

	static void RegisterMetatable(lua_State* L, const char* Name, TArray<luaL_Reg>);
	static void RegisterMethod(lua_State* L, luaL_Reg Reg);
	static bool CheckLua(lua_State* L, int Returned);
	static void StackDump(lua_State* L);

	static int CallUFunction(lua_State* L, UObject* Object, UFunction* Function, int StartIndex);
	static void UFunctionToLua(lua_State* L, UFunction* Function, UObject* Object);

	// Mostly borrowed from FIN's source. Thanks Pana !
	static void PropertyToLua(lua_State* L, FField* Field, void* Container);
	static void LuaToProperty(lua_State* L, FField* Field, void* Container, int Index);

	static int Lua_GetClass(lua_State* L);
	static int Lua_MakeStructInstance(lua_State* L);
	static int Lua_MakeSubclass(lua_State* L);
	static int Lua_UnlockRecipe(lua_State* L);
	static int Lua_LoadObject(lua_State* L);
	static int Lua_Print(lua_State* L);
	static int Lua_Test(lua_State* L);
	static int Lua_WaitForEvent(lua_State* L);
	static int Lua_WaitForMod(lua_State* L);
	static int Lua_DumpFunction(lua_State* L);
	static int Lua_LoadFunction(lua_State* L);
};
