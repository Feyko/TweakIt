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

bool CheckLua(lua_State* L, int Returned) {
	if (Returned != LUA_OK) {
		string ErrorMsg = lua_tostring(L, -1);
		LOGS(ErrorMsg)
		return false;
	} 
	return true;
}

void StackDump(lua_State* L) {
	int Top = lua_gettop(L);
	for (int i = 1; i <= Top; i++) {
		int t = lua_type(L, i);
		string s = lua_typename(L, t);
		LOGS(s);
		LOG(""); // put a separator 
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void PropertyToLua(lua_State* L, UProperty* Property, void* Data) {
	LOGF("Transforming from Property %s to Lua", *Property->GetName());
	auto c = Property->GetClass()->GetCastFlags();
	if (c & EClassCastFlags::CASTCLASS_FBoolProperty) {
		lua_pushboolean(L, *Property->ContainerPtrToValuePtr<bool>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FIntProperty) {
		lua_pushinteger(L, *Property->ContainerPtrToValuePtr<std::int32_t>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FInt64Property) {
		lua_pushinteger(L, *Property->ContainerPtrToValuePtr<std::int64_t>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FFloatProperty) {
		lua_pushnumber(L, *Property->ContainerPtrToValuePtr<float>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FStrProperty) {
		FString String = *Property->ContainerPtrToValuePtr<FString>(Data);
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	} else if (c & EClassCastFlags::CASTCLASS_FNameProperty) {
		FString NameString = Property->ContainerPtrToValuePtr<FName>(Data)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*NameString));
	} else if (c & EClassCastFlags::CASTCLASS_FTextProperty) {
		FString NameString = Property->ContainerPtrToValuePtr<FText>(Data)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*NameString));
	} else if (c & EClassCastFlags::CASTCLASS_FClassProperty) {
		FLuaUClass::ConstructClass(L, *Property->ContainerPtrToValuePtr<UClass*>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FEnumProperty) {
		UEnumProperty* EnumProperty = Cast<UEnumProperty>(Property);
		int64 EnumValue = static_cast<int64>(*EnumProperty->ContainerPtrToValuePtr<uint8>(Data));
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
		UStructProperty* StructProperty = Cast<UStructProperty>(Property);
		void* StructValue = StructProperty->ContainerPtrToValuePtr<void>(Data);
		FLuaUStruct::ConstructStruct(L, StructProperty->Struct, StructValue);
	} else if (c & EClassCastFlags::CASTCLASS_FObjectProperty) {
		FLuaUObject::ConstructObject(L, Property->ContainerPtrToValuePtr<UObject>(Data));
	} else if (c & EClassCastFlags::CASTCLASS_FArrayProperty) {
		UArrayProperty* Prop = Cast<UArrayProperty>(Property);
		FLuaTArray::ConstructArray(L, Prop, Data);
	} else {
		LOG("DIDN'T MATCH ANY CAST FLAGS")
		lua_pushnil(L);
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void LuaToProperty(lua_State* L, UProperty* Property, void* Data, int Index) {
	LOGF("Transforming from Lua to Property %s", *Property->GetName());
	auto c = Property->GetClass()->GetCastFlags();
	if (c & EClassCastFlags::CASTCLASS_FBoolProperty) {
		*Property->ContainerPtrToValuePtr<bool>(Data) = static_cast<bool>(lua_toboolean(L, Index));
	} else if (c & EClassCastFlags::CASTCLASS_FIntProperty) {
		*Property->ContainerPtrToValuePtr<std::int32_t>(Data) = static_cast<std::int32_t>(lua_tointeger(L, Index));
	} else if (c & EClassCastFlags::CASTCLASS_FInt64Property) {
		*Property->ContainerPtrToValuePtr<std::int64_t>(Data) = static_cast<std::int64_t>(lua_tointeger(L, Index));
	} else if (c & EClassCastFlags::CASTCLASS_FFloatProperty) {
		*Property->ContainerPtrToValuePtr<float>(Data) = static_cast<float>(lua_tonumber(L, Index));
	} else if (c & EClassCastFlags::CASTCLASS_FStrProperty) {
		size_t Len;
		const char* s = lua_tolstring(L, Index, &Len);
		if (!s) throw std::exception("Invalid String in string property parse");
		FString* o = Property->ContainerPtrToValuePtr<FString>(Data);
		*o = FString(UTF8_TO_TCHAR(s));
	} else if (c & EClassCastFlags::CASTCLASS_FNameProperty) {
		*Property->ContainerPtrToValuePtr<FName>(Data) = FName(*static_cast<FString>(lua_tostring(L, Index)));
	} else if (c & EClassCastFlags::CASTCLASS_FTextProperty) {
		*Property->ContainerPtrToValuePtr<FText>(Data) = FText::FromString(static_cast<FString>(lua_tostring(L, Index)));
	} else if (c & EClassCastFlags::CASTCLASS_FClassProperty) {
		UClass* o = static_cast<FLuaUClass*>(lua_touserdata(L, Index))->Class;
		*Property->ContainerPtrToValuePtr<UClass*>(Data) = o;
	} else if (c & EClassCastFlags::CASTCLASS_FEnumProperty) {
		FName NameEnumValue = lua_tostring(L, Index);
		UEnum* Enum = Cast<UEnumProperty>(Property)->GetEnum();
		if (Enum->IsValidEnumName(NameEnumValue)) {
			int64 EnumValue = Enum->GetValueByName(NameEnumValue);
			*Property->ContainerPtrToValuePtr<uint8>(Data) = static_cast<uint8>(EnumValue);
		}
	} else if (c & EClassCastFlags::CASTCLASS_FStructProperty) {
		UStructProperty* StructProperty = Cast<UStructProperty>(Property);
		FLuaUStruct* rStruct = static_cast<FLuaUStruct*>(lua_touserdata(L, Index));
		if(StructProperty->Struct->GetFullName() == rStruct->Struct->GetFullName()) {
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


int Lua_GetClass(lua_State* L) {
	LOG("Getting a class");
	FString ClassName = luaL_checkstring(L, 1);
	FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
	UClass* Class = FTIReflection::FindClassByName(ClassName, Package);
	FLuaUClass::ConstructClass(L, Class);
	return 1;
}

int Lua_MakeStructInstance(lua_State* L) {
	LOG("Making a struct instance")
	UStruct* BaseStruct;
	if(lua_isuserdata(L, 1)) {
		BaseStruct = FLuaUStruct::Get(L)->Struct;
	} else {
		FString StructName = luaL_checkstring(L, 1);
		FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
		BaseStruct = FTIReflection::FindStructByName(StructName, Package);
		if(!BaseStruct)
		{
			LOGF("Couldn't find a struct with the name %s", *StructName)
			lua_pushnil(L);
			return 1;
		}
	}
	if(!BaseStruct->IsValidLowLevel()) {
		LOG("Trying to make an instance of an invalid struct")
		lua_pushnil(L);
		return 1;
	}
	void* Instance = FTIReflection::MakeStructInstance(BaseStruct);
	FLuaUStruct::ConstructStruct(L, BaseStruct, Instance, true);
	return 1;
}

int Lua_MakeSubclass(lua_State* L) {
	UClass* ParentClass = FLuaUClass::Get(L)->Class;
	FString Name = luaL_checkstring(L, 2);
	UClass* GeneratedClass = FTIReflection::GenerateUniqueSimpleClass(*("/Game/TweakIt/Generated/" + Name), *Name, ParentClass);
	FLuaUClass::ConstructClass(L, GeneratedClass);
	return 1;
}

int Lua_UnlockRecipe(lua_State* L) {
	UClass* Class = FLuaUClass::Get(L)->Class;
	if(!lua_isuserdata(L, -2)) {
		lua_getglobal(L, "WorldContext");
	}
	UObject* WorldContext = FLuaUObject::Get(L)->Object;
	FTIContentRegistration::UnlockRecipe(Class, WorldContext);
	return 0;
}

int Lua_LoadObject(lua_State* L) {
	LOG("Loading an object")
	FString Path = lua_tostring(L, 1);
	UClass* Class = lua_isuserdata(L, 2) ? FLuaUClass::Get(L, 2)->Class : UObject::StaticClass();
	UObject* Object = StaticLoadObject(Class, nullptr, *Path);
	FLuaUObject::ConstructObject(L, Object);
	return 1;
}

int Lua_Print(lua_State* L) {
	FString String = luaL_checkstring(L, 1);
	LOGFS(String)
	return 0;
}

lua_Number GetNumber(lua_State* L)
{
	return luaL_checknumber(L, 1);
}

int Lua_Test(lua_State* L)
{
	return 0;
}

