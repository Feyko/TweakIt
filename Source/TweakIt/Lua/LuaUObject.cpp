#include "LuaUObject.h"

#include "LuaUClass.h"
#include "Lua.h"
#include <string>

#include "Reflection.h"
using namespace std;

using namespace TweakIt;
using namespace Lua;



int LuaUObject::lua_index(lua_State* L)
{
    
    LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
    FString PropertyName = lua_tostring(L, 2);
    LOGFS(FString::Printf(TEXT("Indexing a LuaUObject with %s"),*PropertyName))
    if(PropertyName == "GetClass") {
        lua_pushcfunction(L, lua_GetClass);
    } else if(PropertyName == "DumpProperties") {
        lua_pushcfunction(L, lua_DumpProperties);
    }
    UProperty* Property = Reflection::FindPropertyByShortName(self->Object->GetClass(), *PropertyName);
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
        
        LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
        FString PropertyName = lua_tostring(L, 2);
        LOGFS(FString::Printf(TEXT("Newindexing a LuaUObject with %s"),*PropertyName))
        UProperty* Property = Reflection::FindPropertyByShortName(self->Object->GetClass(), *PropertyName);
        if(Property->IsValidLowLevel())
        {
            LOG("Found property")
            luaToProperty(L, Property, self->Object, 3);
        }
        else{lua_pushnil(L);}
        return 0;
    }
}

int LuaUObject::lua_DumpProperties(lua_State* L)
{
    LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
    LOGFS(FString("Dumping the properties for " + self->Object->GetName()))
    for(UProperty* Property = self->Object->GetClass()->PropertyLink; Property; Property = Property->PropertyLinkNext) {
        LOGFS(Property->GetName())
    }
    return 0;
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
        UE_LOG(LogScript, Warning, TEXT("[TweakIt] Trying to construct a LuaUObject from an invalid object"))
        lua_pushnil(L);
    }
    return 1;
}

int LuaUObject::lua_gc(lua_State* L)
{
    LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
    self->~LuaUObject();
    return 0;
}

int LuaUObject::lua_GetClass(lua_State* L)
{
    LuaUObject* self = (LuaUObject*)lua_touserdata(L, 1);
    LuaUClass::ConstructClass(L, self->Object->GetClass());
    return 1;
}