#include "LuaUObject.h"

#include "LuaUClass.h"
#include "Lua.h"
#include <string>

#include "Reflection.h"
using namespace std;

using namespace Tweaker;
using namespace Lua;



int LuaUObject::lua_index(lua_State* L)
{
    LOG("Indexing a LuaUObject")
    LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
    string PropertyName = lua_tostring(L, 2);
    LOGS(PropertyName)
    for(UProperty* Property = self->Object->GetClass()->PropertyLink; Property; Property = Property->PropertyLinkNext) {
        LOGFS(Property->GetName())
    }
    UProperty* Property = Reflection::FindPropertyByShortName(self->Object->GetClass(), *FString(PropertyName.c_str()));
    if(Property->IsValidLowLevel())
    {
        LOG("Found property")
        propertyToLua(L, Property, self->Object);
    }
    else{lua_pushnil(L);}
    return 1;
}

int LuaUObject::lua_newindex(lua_State* L)
{
    {
        LOG("In newindex")
        LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
        std::string PropertyName = lua_tostring(L, 2);
        
        UProperty* Property = Reflection::FindPropertyByShortName(self->Object->GetClass(), *FString(PropertyName.c_str()));
        if(Property->IsValidLowLevel())
        {
            LOG("Found property")
            luaToProperty(L, Property, self->Object, 3);
        }
        else{lua_pushnil(L);}
        return 0;
    }
}

int LuaUObject::ConstructObject(lua_State* L, UObject* Object)
{
    LOG("Constructing a LuaUObject")
    if(Object->IsValidLowLevel())
    {
        LuaUObject* ReturnedInstance = static_cast<LuaUObject*>(lua_newuserdata(L, sizeof(LuaUObject)));
        new (ReturnedInstance) LuaUObject{Object};
        luaL_getmetatable(L, "LuaUObjectMeTa");
        lua_setmetatable(L, -2);
    }
    else
    {
        UE_LOG(LogScript, Warning, TEXT("[TWEAKER] Trying to construct a LuaUObject from an invalid object"))
        lua_pushnil(L);
    }
    return 1;
}
