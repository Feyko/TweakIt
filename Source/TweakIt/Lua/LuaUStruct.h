#pragma once
#include "CoreMinimal.h"


#include "TweakIt\LuaLib\lua.hpp"

namespace TweakIt
{
	namespace Lua
	{
		struct LuaUStruct
		{
			UStruct* Struct;
			void* Values;

			static int lua_index(lua_State* L);

			static int lua_newindex(lua_State* L);

			static int lua__tostring(lua_State* L);
			static int lua_gc(lua_State* L);

			static int ConstructStruct(lua_State* L, UStruct* Struct, void* Values);
		};
	}
}
