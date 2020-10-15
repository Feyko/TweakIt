#include "LuaUEnum.h"

#include "LuaUClass.h"
#include "Lua.h"
#include <string>
using namespace std;

using namespace Tweaker::Lua;

int LuaUEnum::lua_index(lua_State* L)
{
    {
        LuaUEnum* self = (LuaUEnum*)lua_touserdata(L, 1);
        string PropertyName = lua_tostring(L, 2);
        return 1;
    }
}

int LuaUEnum::lua_newindex(lua_State* L)
{
    {
        LOG("In newindex")
        LuaUEnum* self = (LuaUEnum*)lua_touserdata(L, 1);
        std::string PropertyName = lua_tostring(L, 2);
        
        return 0;
    }
}

int LuaUEnum::ConstructEnum(lua_State* L, UEnum* Enum)
{
    if(Enum->IsValidLowLevelFast())
    {
        LuaUEnum* ReturnedInstance = static_cast<LuaUEnum*>(lua_newuserdata(L, sizeof(LuaUEnum)));
        new (ReturnedInstance) LuaUEnum{Enum};
        luaL_getmetatable(L, "LuaUEnumMeTa");
        lua_setmetatable(L, -2);
    }
    else
    {
        UE_LOG(LogScript, Warning, TEXT("[TWEAKER] Trying to construct a LuaUClass from an invalid class"))
        lua_pushnil(L);
    }
    return 1;
}
