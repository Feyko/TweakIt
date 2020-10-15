#pragma once
#include "CoreMinimal.h"

#include <string>

#include "D:\SatisfactoryModding\SatisfactoryModLoader\Source\Tweaker\LuaLib\lua.hpp"
namespace Tweaker
{
    namespace Lua
    {
        struct LuaUEnum
        {
            UEnum* Enum;

            static int lua_index(lua_State* L);
    
            static int lua_newindex(lua_State* L);

            static int ConstructEnum(lua_State* L, UEnum* Enum);
        };
    }
}