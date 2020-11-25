#pragma once
#include "CoreMinimal.h"

#include <string>

#include "D:\SatisfactoryModding\SatisfactoryModLoader\Source\TweakIt\LuaLib\lua.hpp"

namespace TweakIt
{
	namespace Lua
	{
		struct LuaUStruct
		{
			UStructProperty* ScriptProperty;
			void* Container;

			static int lua_index(lua_State* L);

			static int lua_newindex(lua_State* L);

			static int lua__tostring(lua_State* L);
			static int lua_gc(lua_State* L);

			static int ConstructStruct(lua_State* L, UStructProperty* StructProperty, void* Container);
		};
	}
}
