#include "LuaUClass.h"
#include "Lua.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "TweakIt/Helpers/TiReflection.h"
#include <string>
#include "LuaUObject.h"
#include "TweakIt/TweakItModule.h"
using namespace std;

int LuaUClass::lua_GetDefaultValue(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	const FString PropertyName = lua_tostring(L, 2);
	LOGF("Getting a LuaUClass's default value for %s", *PropertyName);
	UActorComponent* Component = nullptr;
	if (self->Class->IsChildOf(AActor::StaticClass())) {
		LOG("Class is an AActor, checking for component first")
		Component = FTIReflection::FindDefaultComponentByName(self->Class,UActorComponent::StaticClass(),
															*PropertyName);
		if (Component) {
			LOG("Found component")
			Component->RegisterComponent();
			LOGF("%hhd", Component->IsRegistered())
			Component->AddToRoot();
			self->Class->GetDefaultObject()->AddToRoot();
			LuaUObject::ConstructObject(L, Component);
			return 1;
		}
	}
	UProperty* Property = FTIReflection::FindPropertyByName(self->Class, *PropertyName);
	if (Property->IsValidLowLevel()) {
		LOGF("Found property %s", *Property->GetName())
		PropertyToLua(L, Property, self->Class->GetDefaultObject());
		self->Class->GetDefaultObject()->AddToRoot();
	} else {lua_pushnil(L);}
	return 1;
}

int LuaUClass::lua_index(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	const FString index = lua_tostring(L, 2);
	LOGFS(FString::Printf(TEXT("Indexing a LuaUClass that holds %s with %s"), *self->Class->GetName(), *index))
	if (index == "GetDefaultValue") {
		lua_pushcfunction(L, lua_GetDefaultValue);
	} else if (index == "ChangeDefaultValue") {
		lua_pushcfunction(L, lua_ChangeDefaultValue);
	} else if (index == "GetChildClasses") {
		lua_pushcfunction(L, lua_GetChildClasses);
	} else if (index == "GetObjects") {
		lua_pushcfunction(L, lua_GetObjects);
	} else if (index == "MakeSubclass") {
		lua_pushcfunction(L, lua_MakeSubclass);
	} else if (index == "AddDefaultComponent") {
		lua_pushcfunction(L, lua_AddDefaultComponent);
	} else if (index == "RemoveDefaultComponent") {
		lua_pushcfunction(L, lua_RemoveDefaultComponent);
	} else if (index == "DumpProperties") {
		lua_pushcfunction(L, lua_DumpProperties);
	} else {
		lua_GetDefaultValue(L);
	}
	return 1;
}

int LuaUClass::lua_ChangeDefaultValue(lua_State* L) {
	
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	const std::string PropertyName = lua_tostring(L, 2);
	const bool IsRecursive = static_cast<bool>(lua_toboolean(L, 4));
	LOGFS(FString::Printf(TEXT("Calling ChangeDefaultValue(%hs,value , %hhd) on class %s"),
											PropertyName.c_str(), IsRecursive, *self->Class->GetName()))
	TArray<UClass*> Classes;
	Classes.Add(self->Class);
	if (IsRecursive) { GetDerivedClasses(self->Class, Classes); }
	for (auto Class : Classes) {
		LOG("Changing the default value of a class")
		UProperty* Property = FTIReflection::FindPropertyByName(Class, *FString(PropertyName.c_str()));
		if (Property) {
			LuaToProperty(L, Property, Class->GetDefaultObject(), 3);
			PropertyToLua(L, Property, Class->GetDefaultObject());
			Class->GetDefaultObject()->AddToRoot();
			LOG("Changed the class's CDO. Iterating over objects...")
			for (FObjectIterator It = FObjectIterator(Class); It; ++It) {
				LuaToProperty(L, Property, *It, 3);
			}
			LOG("Finished iteration over objects")
		} else {
			LOG("Couldn't find the property")
			lua_pushnil(L);
			return 0;
		}
	}
	return 0;
}

// WIP Level : Fatal
int LuaUClass::lua_AddDefaultComponent(lua_State* L) {
	LOG("Adding a default component")
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	const FString ComponentName = lua_tostring(L, 2);
	LuaUClass* LuaComponent = static_cast<LuaUClass*>(lua_touserdata(L, 3));
	AActor* Actor = Cast<AActor>(self->Class->GetDefaultObject());
	if (!Actor->IsValidLowLevel()) {
		LOG("Actor wasn't valid");
		lua_pushnil(L);
		return 0;
	}
	if (!LuaComponent->Class->GetDefaultObject()->IsA(UActorComponent::StaticClass())) {
		LOG("Component class wasn't an Actor Component");
		lua_pushnil(L);
		return 0;
	}
	UActorComponent* Component = NewObject<UActorComponent>(Actor, LuaComponent->Class, FName(*ComponentName));
	if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component)) {
		SceneComponent->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	Actor->AddToRoot();
	LuaUObject::ConstructObject(L, Component);
	return 1;
}

// WIP Level : Untested. Guessed level is Fatal
int LuaUClass::lua_RemoveDefaultComponent(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	const FString ComponentName = lua_tostring(L, 2);
	AActor* Actor = Cast<AActor>(self->Class->GetDefaultObject());
	if (!Actor->IsValidLowLevel()) {
		lua_pushnil(L);
		return 0;
	}
	UActorComponent* Component = FTIReflection::FindDefaultComponentByName(
		self->Class,
		UActorComponent::StaticClass(),
		ComponentName);
	if (Component->IsValidLowLevel()) {
		LOG("Destroying the component")
		Component->DestroyComponent();
	}
	return 0;
}

int LuaUClass::lua_GetChildClasses(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	TArray<UClass*> Classes;
	GetDerivedClasses(self->Class, Classes);
	lua_newtable(L);
	for (int i = 0; i < Classes.Num(); i++) {
		ConstructClass(L, Classes[i]);
		lua_seti(L, -2, i + 1);
	}
	return 1;
}

int LuaUClass::lua_GetObjects(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	TArray<UObject*> Objects;
	GetObjectsOfClass(self->Class, Objects);
	lua_newtable(L);
	for (int i = 0; i < Objects.Num(); i++) {
		LuaUObject::ConstructObject(L, Objects[i]);
		lua_seti(L, -2, i + 1);
	}
	return 1;
}

int LuaUClass::lua_DumpProperties(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	LOGF("Dumping the properties for %s", *self->Class->GetName())
	for (UProperty* Property = self->Class->PropertyLink; Property; Property = Property->PropertyLinkNext) {
		LOGFS(Property->GetName())
	}
	return 0;
}

int LuaUClass::lua_newindex(lua_State* L) {
	lua_ChangeDefaultValue(L);
	return 1;
}

int LuaUClass::lua__call(lua_State* L) {
	if (lua_isboolean(L, 4)) {
		lua_pop(L, 1);
	}
	lua_pushboolean(L, true);
	lua_ChangeDefaultValue(L);
	return 0;
}

int LuaUClass::lua__tostring(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*self->Class->GetName()));
	return 1;
}

int LuaUClass::lua_gc(lua_State* L) {
	LuaUClass* self = static_cast<LuaUClass*>(lua_touserdata(L, 1));
	self->~LuaUClass();
	return 0;
}

int LuaUClass::ConstructClass(lua_State* L, UClass* Class) {
	if (Class && Class->IsValidLowLevel()) {
		LOGF("Constructing a LuaUClass from %s", *Class->GetName())
		LuaUClass* ReturnedInstance = static_cast<LuaUClass*>(lua_newuserdata(L, sizeof(LuaUClass)));
		new(ReturnedInstance) LuaUClass{Class};
		luaL_getmetatable(L, "LuaUClassMeTa");
		lua_setmetatable(L, -2);
	} else {
		LOG("Trying to construct a LuaUClass from an invalid class")
		lua_pushnil(L);
	}
	return 1;
}
