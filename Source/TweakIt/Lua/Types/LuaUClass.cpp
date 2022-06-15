#include "LuaUClass.h"
#include "TweakIt/Lua/Lua.h"
#include "TweakIt/Helpers/TiReflection.h"
#include <string>
#include "LuaUObject.h"
#include "TweakIt/Logging/FTILog.h"
using namespace std;

int FLuaUClass::ConstructClass(lua_State* L, UClass* Class)
{
	if (!Class->IsValidLowLevel())
	{
		LOG("Trying to construct a LuaUClass from an invalid class")
		lua_pushnil(L);
		return 1;
	}
	LOGF("Constructing a LuaUClass from %s", *Class->GetName())
	FLuaUClass* ReturnedInstance = static_cast<FLuaUClass*>(lua_newuserdata(L, sizeof(FLuaUClass)));
	new(ReturnedInstance) FLuaUClass{Class};
	luaL_getmetatable(L, FLuaUClass::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaUClass* FLuaUClass::Get(lua_State* L, int Index)
{
	return static_cast<FLuaUClass*>(luaL_checkudata(L, Index, Name));
}

int FLuaUClass::Lua_GetDefaultValue(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	const FString PropertyName = luaL_checkstring(L, 2);
	LOGF("Getting a LuaUClass's default value for %s", *PropertyName);
	if (Self->Class->IsChildOf(AActor::StaticClass()))
	{
		LOG("Class is an AActor, checking for component first")
		if (UActorComponent* Component = FTIReflection::FindDefaultComponentByName(
			Self->Class, UActorComponent::StaticClass(),
			*PropertyName))
		{
			LOG("Found component")
			Component->RegisterComponent();
			LOGF("%hhd", Component->IsRegistered())
			Component->AddToRoot();
			FLuaUObject::ConstructObject(L, Component);
			return 1;
		}
	}
	UProperty* Property = FTIReflection::FindPropertyByName(Self->Class, *PropertyName);
	if (!Property->IsValidLowLevel())
	{
		lua_pushnil(L);
		return 1;
	}
	LOGF("Found property %s", *Property->GetName())
	PropertyToLua(L, Property, Self->Class->GetDefaultObject());
	return 1;
}

int FLuaUClass::Lua_ChangeDefaultValue(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	FString PropertyName = luaL_checkstring(L, 2);
	bool IsRecursive = static_cast<bool>(lua_toboolean(L, 4));
	LOGF("Calling ChangeDefaultValue(%s, <value>, %hhd) on class %s", *PropertyName, IsRecursive,
	     *Self->Class->GetName())
	TArray<UClass*> Classes;
	Classes.Add(Self->Class);
	if (IsRecursive)
	{
		GetDerivedClasses(Self->Class, Classes);
	}
	for (auto Class : Classes)
	{
		LOG("Changing the default value of a class")
		UProperty* Property = FTIReflection::FindPropertyByName(Class, *PropertyName);
		if (!Property->IsValidLowLevel())
		{
			LOG("Couldn't find the property")
			return 0;
		}
		LuaToProperty(L, Property, Class->GetDefaultObject(), 3);
		LOG("Changed the class's CDO. Iterating over objects...")
		for (FObjectIterator It = FObjectIterator(Class); It; ++It)
		{
			LuaToProperty(L, Property, *It, 3);
		}
		LOG("Finished iteration over objects")
	}
	return 0;
}

// WIP Level : Fatal
int FLuaUClass::Lua_AddDefaultComponent(lua_State* L)
{
	LOG("Adding a default component")
	FLuaUClass* Self = Get(L);
	const FString ComponentName = luaL_checkstring(L, 2);
	FLuaUClass* LuaComponent = Get(L, 3);
	AActor* Actor = Cast<AActor>(Self->Class->GetDefaultObject());
	if (!Actor->IsValidLowLevel())
	{
		LOG("Actor wasn't valid");
		lua_pushnil(L);
		return 1;
	}
	if (!LuaComponent->Class->GetDefaultObject()->IsA(UActorComponent::StaticClass()))
	{
		LOG("Component class wasn't an Actor Component");
		lua_pushnil(L);
		return 1;
	}
	UActorComponent* Component = NewObject<UActorComponent>(Actor, LuaComponent->Class, FName(*ComponentName));
	if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
	{
		SceneComponent->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	FLuaUObject::ConstructObject(L, Component);
	return 1;
}

// WIP Level : Untested. Guessed level is Fatal
int FLuaUClass::Lua_RemoveDefaultComponent(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	const FString ComponentName = luaL_checkstring(L, 2);
	AActor* Actor = Cast<AActor>(Self->Class->GetDefaultObject());
	if (!Actor->IsValidLowLevel())
	{
		return 0;
	}
	UActorComponent* Component = FTIReflection::FindDefaultComponentByName(
		Self->Class,
		UActorComponent::StaticClass(),
		ComponentName);
	if (Component->IsValidLowLevel())
	{
		LOG("Destroying the component")
		Component->DestroyComponent();
	}
	return 0;
}

int FLuaUClass::Lua_GetChildClasses(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	TArray<UClass*> Classes;
	GetDerivedClasses(Self->Class, Classes);
	lua_newtable(L);
	for (int i = 0; i < Classes.Num(); i++)
	{
		ConstructClass(L, Classes[i]);
		lua_seti(L, -2, i + 1);
	}
	return 1;
}

int FLuaUClass::Lua_GetObjects(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	TArray<UObject*> Objects;
	GetObjectsOfClass(Self->Class, Objects);
	lua_newtable(L);
	for (int i = 0; i < Objects.Num(); i++)
	{
		FLuaUObject::ConstructObject(L, Objects[i]);
		lua_seti(L, -2, i + 1);
	}
	return 1;
}

int FLuaUClass::Lua_DumpProperties(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	LOGF("Dumping the properties for %s", *Self->Class->GetName())
	for (UProperty* Property = Self->Class->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		LOG(Property->GetName())
	}
	return 0;
}

int FLuaUClass::Lua__index(lua_State* L)
{
	FLuaUClass* Self = Get(L);
	const FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaUClass that holds %s with %s", *Self->Class->GetName(), *Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	return Lua_GetDefaultValue(L);
}

int FLuaUClass::Lua__newindex(lua_State* L)
{
	Lua_ChangeDefaultValue(L);
	return 1;
}

int FLuaUClass::Lua__call(lua_State* L)
{
	if (lua_isboolean(L, 4))
	{
		lua_pop(L, 1);
	}
	lua_pushboolean(L, true);
	Lua_ChangeDefaultValue(L);
	return 0;
}

int FLuaUClass::Lua__tostring(lua_State* L)
{
	FLuaUClass* Self = static_cast<FLuaUClass*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Class->GetName()));
	return 1;
}

void FLuaUClass::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}
