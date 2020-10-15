#include "LuaUClass.h"
#include "Lua.h"
#include "util/ReflectionHelper.h"
#include "Reflection.h"
#include <string>

#include "LuaUObject.h"
using namespace std;

using namespace Tweaker;
using namespace Lua;

int LuaUClass::lua_GetDefaultValue(lua_State* L)
{
    LOG("Getting a LuaUClass's default value")
    stackDump(L);
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    string PropertyName = lua_tostring(L, 2);
    UProperty* Property = nullptr;
    UActorComponent* Component = nullptr;
    if(self->Class->IsChildOf(AActor::StaticClass()))
    {
        LOG("Class is an AActor, checking for component first")
        Component = Reflection::FindDefaultComponentByName(self->Class, UActorComponent::StaticClass(), *FString(PropertyName.c_str()));
        if(Component)
        {
            LOG("Found component")
            LuaUObject::ConstructObject(L, Component);
        }
        else{lua_pushnil(L);}
    }
    Property = Reflection::FindPropertyByShortName(self->Class, *FString(PropertyName.c_str()));
    if(!Component)
    {
        if(Property->IsValidLowLevel())
        {
            LOG("Found property")
            propertyToLua(L, Property, self->Class->GetDefaultObject());
            self->Class->GetDefaultObject()->AddToRoot();
        }
        else{lua_pushnil(L);}
    }
    return 1;
}

int LuaUClass::lua_index(lua_State* L)
{
    LOG("Indexing a LuaUClass")
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    string index = lua_tostring(L, 2);
    if(index == "GetDefaultValue") {
        lua_pushcfunction(L, lua_GetDefaultValue);
    } else if(index == "ChangeDefaultValue") {
        lua_pushcfunction(L, lua_ChangeDefaultValue);
    } else if(index == "AddDefaultComponent") {
        lua_pushcfunction(L, lua_AddDefaultComponent);
    } else if(index == "RemoveDefaultComponent") {
        lua_pushcfunction(L, lua_RemoveDefaultComponent);
    }  else if(index == "DumpProperties") {
        lua_pushcfunction(L, lua_DumpProperties);
    } else {
        lua_GetDefaultValue(L);
    }
    return 1;
}

int LuaUClass::lua_ChangeDefaultValue(lua_State* L)
{
    LOG("Changing value ???")
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    std::string PropertyName = lua_tostring(L, 2);
    bool IsRecursive = (bool)lua_toboolean(L, 4);
    TArray<UClass*> Classes;
    Classes.Add(self->Class);
    if(IsRecursive) {GetDerivedClasses(self->Class, Classes);}
    for(auto Class: Classes)
    {
        UProperty* Property = FReflectionHelper::FindPropertyByShortName<UProperty>(self->Class, *FString(PropertyName.c_str()));
        if(Property)
        {
            luaToProperty(L, Property, self->Class->GetDefaultObject(), 3);
            self->Class->GetDefaultObject()->AddToRoot();
        }
        else
        {
            lua_pushnil(L);
            return 1;
        }
    }
    lua_pushvalue(L, 3);
    return 1;
}

int LuaUClass::lua_AddDefaultComponent(lua_State* L)
{
    LOG("Adding a default component")
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    FString ComponentName = lua_tostring(L, 2);
    LuaUClass* LuaComponent = (LuaUClass*)lua_touserdata(L, 3);
    AActor* Actor = Cast<AActor>(self->Class->GetDefaultObject());
    if(!Actor->IsValidLowLevel()) {LOG("Actor wasn't valid"); lua_pushnil(L); return 0;}
    if(!LuaComponent->Class->GetDefaultObject()->IsA(UActorComponent::StaticClass())) {LOG("Component class wasn't an Actor Component");lua_pushnil(L); return 0;}
    UActorComponent* Component = NewObject<UActorComponent>(Actor, LuaComponent->Class, FName(*ComponentName));
    if(USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
    {
        SceneComponent->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
    Actor->AddToRoot();
    LuaUObject::ConstructObject(L, Component);
    return 1;
}

int LuaUClass::lua_RemoveDefaultComponent(lua_State* L)
{
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    FString ComponentName = lua_tostring(L, 2);
    AActor* Actor = Cast<AActor>(self->Class->GetDefaultObject());
    if(!Actor->IsValidLowLevel()) {lua_pushnil(L); return 0;}
    UActorComponent* Component = Reflection::FindDefaultComponentByName(self->Class, UActorComponent::StaticClass(), ComponentName);
    if(Component->IsValidLowLevel())
    {
        LOG("Destroying the component")
        Component->DestroyComponent();
    }
    return 0;
}

int LuaUClass::lua_DumpProperties(lua_State* L)
{
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    LOGFS(FString("Dumping the properties for " + self->Class->GetName()))
    for(UProperty* Property = self->Class->PropertyLink; Property; Property = Property->PropertyLinkNext) {
        LOGFS(Property->GetName())
    }
    return 0;
}

int LuaUClass::lua_newindex(lua_State* L)
{
    lua_ChangeDefaultValue(L);
    return 1;
}

int LuaUClass::lua__call(lua_State* L)
{
    if(lua_isboolean(L,4))
    {
        lua_pop(L,1);
    }
    lua_pushboolean(L, true);
    lua_ChangeDefaultValue(L);
    return 0;
}

int LuaUClass::lua__tostring(lua_State* L)
{
    LuaUClass* self = (LuaUClass*)lua_touserdata(L, 1);
    lua_pushstring(L, TCHAR_TO_UTF8(*self->Class->GetName()));
    return 1;
}

int LuaUClass::ConstructClass(lua_State* L, UClass* Class)
{
    if(Class->IsValidLowLevelFast())
    {
        LuaUClass* ReturnedInstance = static_cast<LuaUClass*>(lua_newuserdata(L, sizeof(LuaUClass)));
        new (ReturnedInstance) LuaUClass{Class};
        luaL_getmetatable(L, "LuaUClassMeTa");
        lua_setmetatable(L, -2);
    }
    else
    {
        UE_LOG(LogScript, Warning, TEXT("[TWEAKER] Trying to construct a LuaUClass from an invalid class"))
        LOG("Super")
        lua_pushnil(L);
    }
    return 1;
}
