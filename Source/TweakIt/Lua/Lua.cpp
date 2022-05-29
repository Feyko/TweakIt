#include "Lua.h"
#include <string>
#include "CoreMinimal.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "FGRecipeManager.h"
#include "IPlatformFilePak.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Helpers/TIContentRegistration.h"

using namespace std;

void RegisterMetatable(lua_State* L, const char* Name, TArray<luaL_Reg> Regs)
{
	luaL_newmetatable(L, Name);
	for (auto Reg : Regs)
	{
		RegisterMethod(L, Reg);
	}
}

void RegisterMethod(lua_State* L, luaL_Reg Reg)
{
	lua_pushstring(L, Reg.name);
	lua_pushcfunction(L, Reg.func);
	lua_settable(L, -3);
}

bool CheckLua(lua_State* L, int r) {
	if (r != LUA_OK) {
		string errormsg = lua_tostring(L, -1);
		LOGS(errormsg)
		return false;
	} 
	return true;
}

void StackDump(lua_State* L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {
		/* repeat for each level */
		int t = lua_type(L, i);
		string s = lua_typename(L, t);
		LOGS(s);
		LOG(""); /* put a separator */
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void PropertyToLua(lua_State* L, UProperty* p, void* data) {
	LOGF("Transforming from Property %s to Lua", *p->GetName());
	auto c = p->GetClass()->GetCastFlags();
	if (c & EClassCastFlags::CASTCLASS_FBoolProperty) {
		lua_pushboolean(L, *p->ContainerPtrToValuePtr<bool>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FIntProperty) {
		lua_pushinteger(L, *p->ContainerPtrToValuePtr<std::int32_t>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FInt64Property) {
		lua_pushinteger(L, *p->ContainerPtrToValuePtr<std::int64_t>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FFloatProperty) {
		lua_pushnumber(L, *p->ContainerPtrToValuePtr<float>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FStrProperty) {
		FString string = *p->ContainerPtrToValuePtr<FString>(data);
		lua_pushstring(L, TCHAR_TO_UTF8(*string));
	} else if (c & EClassCastFlags::CASTCLASS_FNameProperty) {
		FString NameString = p->ContainerPtrToValuePtr<FName>(data)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*NameString));
	} else if (c & EClassCastFlags::CASTCLASS_FTextProperty) {
		FString NameString = p->ContainerPtrToValuePtr<FText>(data)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*NameString));
	} else if (c & EClassCastFlags::CASTCLASS_FClassProperty) {
		LuaUClass::ConstructClass(L, *p->ContainerPtrToValuePtr<UClass*>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FEnumProperty) {
		UEnumProperty* EnumProperty = Cast<UEnumProperty>(p);
		int64 EnumValue = static_cast<int64>(*EnumProperty->ContainerPtrToValuePtr<uint8>(data));
		UEnum* Enum = EnumProperty->GetEnum();
		if (Enum->IsValidEnumValue(EnumValue)) {
			FName Name = Enum->GetNameByValue(EnumValue);
			FString StringName = Name.ToString();
			lua_pushstring(L, TCHAR_TO_UTF8(*StringName));
		} else {
			LOG("Enum value wasn't valid. Please report this to Feyko")
			lua_pushnil(L);
		}
	} else if (c & EClassCastFlags::CASTCLASS_FStructProperty) {
		UStructProperty* StructProperty = Cast<UStructProperty>(p);
		void* StructValue = StructProperty->ContainerPtrToValuePtr<void>(data);
		LuaUStruct::ConstructStruct(L, StructProperty->Struct, StructValue);
	} else if (c & EClassCastFlags::CASTCLASS_FObjectProperty) {
		LuaUObject::ConstructObject(L, p->ContainerPtrToValuePtr<UObject>(data));
	} else if (c & EClassCastFlags::CASTCLASS_FArrayProperty) {
		UArrayProperty* prop = Cast<UArrayProperty>(p);
		LuaTArray::ConstructArray(L, prop, data);
	} else {
		LOG("DIDN'T MATCH ANY CAST FLAGS")
		lua_pushnil(L);
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void LuaToProperty(lua_State* L, UProperty* p, void* data, int i) {
	LOGF("Transforming from Lua to Property %s", *p->GetName());
	auto c = p->GetClass()->GetCastFlags();
	if (c & EClassCastFlags::CASTCLASS_FBoolProperty) {
		*p->ContainerPtrToValuePtr<bool>(data) = static_cast<bool>(lua_toboolean(L, i));
	} else if (c & EClassCastFlags::CASTCLASS_FIntProperty) {
		*p->ContainerPtrToValuePtr<std::int32_t>(data) = static_cast<std::int32_t>(lua_tointeger(L, i));
	} else if (c & EClassCastFlags::CASTCLASS_FInt64Property) {
		*p->ContainerPtrToValuePtr<std::int64_t>(data) = static_cast<std::int64_t>(lua_tointeger(L, i));
	} else if (c & EClassCastFlags::CASTCLASS_FFloatProperty) {
		*p->ContainerPtrToValuePtr<float>(data) = static_cast<float>(lua_tonumber(L, i));
	} else if (c & EClassCastFlags::CASTCLASS_FStrProperty) {
		size_t len;
		const char* s = lua_tolstring(L, i, &len);
		if (!s) throw std::exception("Invalid String in string property parse");
		FString* o = p->ContainerPtrToValuePtr<FString>(data);
		*o = FString(UTF8_TO_TCHAR(s));
	} else if (c & EClassCastFlags::CASTCLASS_FNameProperty) {
		*p->ContainerPtrToValuePtr<FName>(data) = FName(*static_cast<FString>(lua_tostring(L, i)));
	} else if (c & EClassCastFlags::CASTCLASS_FTextProperty) {
		*p->ContainerPtrToValuePtr<FText>(data) = FText::FromString(static_cast<FString>(lua_tostring(L, i)));
	} else if (c & EClassCastFlags::CASTCLASS_FClassProperty) {
		UClass* o = ((LuaUClass*)lua_touserdata(L, i))->Class;
		*p->ContainerPtrToValuePtr<UClass*>(data) = o;
	} else if (c & EClassCastFlags::CASTCLASS_FEnumProperty) {
		FName NameEnumValue = lua_tostring(L, i);
		UEnum* Enum = Cast<UEnumProperty>(p)->GetEnum();
		if (Enum->IsValidEnumName(NameEnumValue)) {
			int64 EnumValue = Enum->GetValueByName(NameEnumValue);
			*p->ContainerPtrToValuePtr<uint8>(data) = static_cast<uint8>(EnumValue);
		}
	} else if (c & EClassCastFlags::CASTCLASS_FStructProperty) {
		UStructProperty* StructProperty = Cast<UStructProperty>(p);
		LuaUStruct* rStruct = static_cast<LuaUStruct*>(lua_touserdata(L, i));
		if(StructProperty->Struct->GetFullName() == rStruct->Struct->GetFullName()) {
			void* StructValue = StructProperty->ContainerPtrToValuePtr<void>(data);
			StructProperty->CopyCompleteValue(StructProperty, rStruct->Values);
		}
		else {
			LOG("The struct you're trying to pass isn't of the same type as the struct you want to overwrite")
		}
	} else if (c & EClassCastFlags::CASTCLASS_FArrayProperty) {
		LOG("Array assignation isn't supported for now. Please edit the array's values instead")
	} else {
		LOG("Property type not supported. Please report this to Feyko")
		lua_pushnil(L);
	}
}


int lua_GetClass(lua_State* L) {
	LOG("Getting a class");
	FString ClassName = lua_tostring(L, 1);
	FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
	UClass* Class = FTIReflection::FindClassByName(ClassName, Package);
	LuaUClass::ConstructClass(L, Class);
	return 1;
}

int lua_MakeStructInstance(lua_State* L) {
	LOG("Making a struct instance")
	UStruct* BaseStruct = nullptr;
	if(lua_isuserdata(L, 1)) {
		LuaUStruct* Struct = static_cast<LuaUStruct*>(lua_touserdata(L, 1));
		BaseStruct = Struct->Struct;
	} else {
		FString StructName = lua_tostring(L, 1);
		FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
		BaseStruct = FTIReflection::FindStructByName(StructName, Package);
		if(!BaseStruct) {LOGF("Couldn't find a struct with the name %s", *StructName) return 1;}
	}
	if(!BaseStruct->IsValidLowLevel()) {
		LOG("Trying to make an instance of an invalid struct")
		return 1;
	}
	void* instance = FTIReflection::MakeStructInstance(BaseStruct);
	LuaUStruct::ConstructStruct(L, BaseStruct, instance);
	return 1;
}

int lua_MakeSubclass(lua_State* L) {
	UClass* ParentClass = static_cast<LuaUClass*>(lua_touserdata(L, 1))->Class;
	FString Name = lua_tostring(L, 2);
	UClass* GeneratedClass = FTIReflection::GenerateUniqueSimpleClass(*("/Game/TweakIt/Generated/" + Name), *Name, ParentClass);
	LuaUClass::ConstructClass(L, GeneratedClass);
	return 1;
}

int lua_UnlockRecipe(lua_State* L) {
	UClass* Class = static_cast<LuaUClass*>(lua_touserdata(L, 1))->Class;
	if(!lua_isuserdata(L, -2)) {
		lua_getglobal(L, "WorldContext");
	}
	UObject* WorldContext = static_cast<LuaUObject*>(lua_touserdata(L, -1))->Object;
	FTIContentRegistration::UnlockRecipe(Class, WorldContext);
	return 0;
}

int lua_LoadObject(lua_State* L) {
	LOG("Loading an object")
	FString path = lua_tostring(L, 1);
	UClass* Class = lua_isuserdata(L, 1) ? ((LuaUClass*)lua_touserdata(L, 2))->Class : UObject::StaticClass();
	UObject* object = StaticLoadObject(Class, nullptr, *path);
	LuaUObject::ConstructObject(L, object);
	return 1;
}

int lua_Print(lua_State* L) {
	FString String = luaL_tolstring(L, -1, NULL);
	lua_pop(L, -1);
	LOGFS(String)
	return 0;
}
