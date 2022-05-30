#pragma once

#include "CoreMinimal.h"
#include "lib/lua.hpp"
#include "Types/include.h"

void RegisterMetatable(lua_State* L, const char* Name, TArray<luaL_Reg>);

void RegisterMethod(lua_State* L, luaL_Reg Reg);

bool CheckLua(lua_State* L, int Returned);

void StackDump(lua_State* L);

// Mostly borrowed from FIN's source. Thanks Pana !
void PropertyToLua(lua_State* L, UProperty* Property, void* Data);

// Mostly borrowed from FIN's source. Thanks Pana !
void LuaToProperty(lua_State* L, UProperty* Property, void* Data, int Index);

int Lua_GetClass(lua_State* L);

int Lua_MakeStructInstance(lua_State* L);

int Lua_MakeSubclass(lua_State* L);

int Lua_UnlockRecipe(lua_State* L);

int Lua_LoadObject(lua_State* L);

int Lua_Print(lua_State* L);

int Lua_Test(lua_State* L);