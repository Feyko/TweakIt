#pragma once
#include "CoreMinimal.h"

#include <string>

#include "D:\SatisfactoryModding\SatisfactoryModLoader\Source\Tweaker\LuaLib\lua.hpp"
namespace Tweaker
{
    namespace Lua
    {
        struct LuaUObject
        {
            UObject* Object;

            static int lua_index(lua_State* L);
    
            static int lua_newindex(lua_State* L);

            static int ConstructObject(lua_State* L, UObject* Object);
        };
    }
}