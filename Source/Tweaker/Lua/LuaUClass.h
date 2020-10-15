#pragma once
#include "CoreMinimal.h"

#include <string>

#include "D:\SatisfactoryModding\SatisfactoryModLoader\Source\Tweaker\LuaLib\lua.hpp"
namespace Tweaker
{
    namespace Lua
    {
        struct LuaUClass
        {
            UClass* Class;

            static int lua_GetDefaultValue(lua_State* L);
            static int lua_index(lua_State* L);
            static int lua_ChangeDefaultValue(lua_State* L);
            static int lua_AddDefaultComponent(lua_State* L);
            static int lua_RemoveDefaultComponent(lua_State* L);
            static int lua_DumpProperties(lua_State* L);

            static int lua_newindex(lua_State* L);

            static int lua__call(lua_State* L);
            static int lua__tostring(lua_State* L);

            static int ConstructClass(lua_State* L, UClass* Class);
        };
    }
}