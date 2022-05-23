﻿#pragma once

#include "CoreMinimal.h"
#include "lib/lua.h"


bool CheckLua(lua_State* L, int r);

void StackDump(lua_State* L);

// Mostly borrowed from FIN's source. Thanks Pana !
void PropertyToLua(lua_State* L, UProperty* p, void* data);

// Mostly borrowed from FIN's source. Thanks Pana !
void LuaToProperty(lua_State* L, UProperty* p, void* data, int i);

int lua_GetClass(lua_State* L);

int lua_MakeStructInstance(lua_State* L);

int lua_MakeSubclass(lua_State* L);

int lua_UnlockRecipe(lua_State* L);

int lua_LoadObject(lua_State* L);

int lua_Print(lua_State* L);
