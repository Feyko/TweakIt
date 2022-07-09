#pragma once

#include "CoreMinimal.h"
#include "lib/lua.hpp"
#include "lib/lua.h"
#include "Types/include.h"

class FTILua
{
public:
	static void luaT_CheckLuaFunction(lua_State* L, int Index);

	static void RegisterMetatable(lua_State* L, const char* Name, TArray<luaL_Reg>);
	static void RegisterMethod(lua_State* L, luaL_Reg Reg);
	static bool CheckLua(lua_State* L, int Returned);
	static void StackDump(lua_State* L);

	static int CallUFunction(lua_State* L, UObject* Object, UFunction* Function);

	// Mostly borrowed from FIN's source. Thanks Pana !
	static void PropertyToLua(lua_State* L, UProperty* Property, void* Container);
	static void LuaToProperty(lua_State* L, UProperty* Property, void* Container, int Index);

	static int Lua_GetClass(lua_State* L);
	static int Lua_MakeStructInstance(lua_State* L);
	static int Lua_MakeSubclass(lua_State* L);
	static int Lua_UnlockRecipe(lua_State* L);
	static int Lua_LoadObject(lua_State* L);
	static int Lua_Print(lua_State* L);
	static int Lua_Test(lua_State* L);
	static int Lua_WaitForEvent(lua_State* L);
	static int Lua_DumpFunction(lua_State* L);
	static int Lua_LoadFunction(lua_State* L);
};
