#pragma once

#include "CoreMinimal.h"
#include "TweakIt/LuaLib/lua.hpp"

namespace TweakIt
{
	namespace Lua
	{
		bool CheckLua(lua_State* L, int r);

		void stackDump(lua_State* L);

		// Mostly borrowed from FIN's source. Thanks Pana !
		void propertyToLua(lua_State* L, UProperty* p, void* data);

		// Mostly borrowed from FIN's source. Thanks Pana !
		void luaToProperty(lua_State* L, UProperty* p, void* data, int i);

		int lua_GetClass(lua_State* L);

		int lua_MakeStructInstance(lua_State* L);
		
		int lua_MakeSubclass(lua_State* L);

		int lua_UnlockRecipe(lua_State* L);

		int lua_LoadObject(lua_State* L);

		int lua_Print(lua_State* L);
	}
}
