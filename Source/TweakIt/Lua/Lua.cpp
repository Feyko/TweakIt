#include "Lua.h"
#include <string>
#include "CoreMinimal.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "FGRecipeManager.h"
#include "FTILuaFuncManager.h"
#include "IPlatformFilePak.h"
#include "LuaLifecycleNotifier.h"
#include "TweakIt/Logging/FTILog.h"
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

bool CheckLua(lua_State* L, int Returned)
{
	if (Returned != LUA_OK)
	{
		string ErrorMsg = lua_tostring(L, -1);
		LOG(ErrorMsg)
		return false;
	}
	return true;
}

void StackDump(lua_State* L)
{
	int Top = lua_gettop(L);
	for (int i = 1; i <= Top; i++)
	{
		int t = lua_type(L, i);
		string s = lua_typename(L, t);
		LOG(s);
		LOG(""); // put a separator 
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void PropertyToLua(lua_State* L, UProperty* Property, void* Container)
{
	LOGF("Transforming from Property %s to Lua", *Property->GetName());
	auto c = Property->GetClass()->GetCastFlags();
	if (c & CASTCLASS_FBoolProperty)
	{
		lua_pushboolean(L, *Property->ContainerPtrToValuePtr<bool>(Container));
	}
	else if (c & CASTCLASS_FIntProperty)
	{
		lua_pushinteger(L, *Property->ContainerPtrToValuePtr<std::int32_t>(Container));
	}
	else if (c & CASTCLASS_FInt64Property)
	{
		lua_pushinteger(L, *Property->ContainerPtrToValuePtr<std::int64_t>(Container));
	}
	else if (c & CASTCLASS_FFloatProperty)
	{
		lua_pushnumber(L, *Property->ContainerPtrToValuePtr<float>(Container));
	}
	else if (c & CASTCLASS_FStrProperty)
	{
		FString String = *Property->ContainerPtrToValuePtr<FString>(Container);
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (c & CASTCLASS_FNameProperty)
	{
		FString String = Property->ContainerPtrToValuePtr<FName>(Container)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (c & CASTCLASS_FTextProperty)
	{
		FString String = Property->ContainerPtrToValuePtr<FText>(Container)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (c & CASTCLASS_FClassProperty)
	{
		FLuaUClass::ConstructClass(L, *Property->ContainerPtrToValuePtr<UClass*>(Container));
	}
	else if (c & CASTCLASS_FEnumProperty)
	{
		UEnumProperty* EnumProperty = Cast<UEnumProperty>(Property);
		int64 EnumValue = *EnumProperty->ContainerPtrToValuePtr<uint8>(Container);
		UEnum* Enum = EnumProperty->GetEnum();
		if (!Enum->IsValidEnumValue(EnumValue))
		{
			luaL_error(L, "Enum value wasn't valid. Please report this to Feyko");
		}
		FName Name = Enum->GetNameByValue(EnumValue);
		FString StringName = Name.ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*StringName));
	}
	else if (c & CASTCLASS_FStructProperty)
	{
		UStructProperty* StructProperty = Cast<UStructProperty>(Property);
		void* StructValue = StructProperty->ContainerPtrToValuePtr<void>(Container);
		FLuaUStruct::ConstructStruct(L, StructProperty->Struct, StructValue);
	}
	else if (c & CASTCLASS_FObjectProperty)
	{
		FLuaUObject::ConstructObject(L, Property->ContainerPtrToValuePtr<UObject>(Container));
	}
	else if (c & CASTCLASS_FArrayProperty)
	{
		UArrayProperty* Prop = Cast<UArrayProperty>(Property);
		FLuaTArray::ConstructArray(L, Prop, Container);
	}
	else
	{
		LOG("DIDN'T MATCH ANY CAST FLAGS")
		lua_pushnil(L);
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void LuaToProperty(lua_State* L, UProperty* Property, void* Container, int Index)
{
	LOGF("Transforming from Lua to Property %s", *Property->GetName());
	uint64 Flags = Property->GetClass()->GetCastFlags();
	if (Flags & CASTCLASS_FBoolProperty)
	{
		luaL_argexpected(L, lua_isboolean(L, Index), Index, "boolean");
		*Property->ContainerPtrToValuePtr<bool>(Container) = static_cast<bool>(lua_toboolean(L, Index));
	}
	else if (Flags & CASTCLASS_FIntProperty)
	{
		*Property->ContainerPtrToValuePtr<std::int32_t>(Container) = static_cast<std::int32_t>(
			luaL_checkinteger(L, Index));
	}
	else if (Flags & CASTCLASS_FInt64Property)
	{
		*Property->ContainerPtrToValuePtr<std::int64_t>(Container) = luaL_checkinteger(L, Index);
	}
	else if (Flags & CASTCLASS_FFloatProperty)
	{
		*Property->ContainerPtrToValuePtr<float>(Container) = static_cast<float>(luaL_checknumber(L, Index));
	}
	else if (Flags & CASTCLASS_FStrProperty)
	{
		FString String = luaL_checkstring(L, Index);
		*Property->ContainerPtrToValuePtr<FString>(Container) = String;
	}
	else if (Flags & CASTCLASS_FNameProperty)
	{
		*Property->ContainerPtrToValuePtr<FName>(Container) = luaL_checkstring(L, Index);
	}
	else if (Flags & CASTCLASS_FTextProperty)
	{
		*Property->ContainerPtrToValuePtr<FText>(Container) = FText::FromString(luaL_checkstring(L, Index));
	}
	else if (Flags & CASTCLASS_FClassProperty)
	{
		FLuaUClass* LuaUClass = FLuaUClass::Get(L, Index);
		*Property->ContainerPtrToValuePtr<UClass*>(Container) = LuaUClass->Class;
	}
	else if (Flags & CASTCLASS_FEnumProperty)
	{
		FName EnumValueName = luaL_checkstring(L, Index);
		UEnum* Enum = Cast<UEnumProperty>(Property)->GetEnum();
		if (!Enum->IsValidEnumName(EnumValueName))
		{
			luaL_error(L, "invalid enum value %s for enum type %s. Example value: %s",
			           TCHAR_TO_UTF8(*EnumValueName.ToString()), TCHAR_TO_UTF8(*Enum->GetName()),
			           TCHAR_TO_UTF8(*Enum->GetNameByIndex(0).ToString()));
			return;
		}
		int64 EnumValue = Enum->GetValueByName(EnumValueName);
		*Property->ContainerPtrToValuePtr<uint8>(Container) = static_cast<uint8>(EnumValue);
	}
	else if (Flags & CASTCLASS_FStructProperty)
	{
		UStructProperty* StructProperty = Cast<UStructProperty>(Property);
		FLuaUStruct* rStruct = FLuaUStruct::Get(L, Index);
		if (!(StructProperty->Struct->GetFullName() == rStruct->Struct->GetFullName()))
		{
			luaL_error(L, "Mismatched struct types (%s <- %s)",
			           TCHAR_TO_UTF8(*StructProperty->Struct->GetName()), TCHAR_TO_UTF8(*rStruct->Struct->GetName()));
			return;
		}
		StructProperty->CopyCompleteValue_InContainer(Container, rStruct->Values);
		void* NewValue = StructProperty->ContainerPtrToValuePtr<void>(Container);
		rStruct->Values = NewValue;
		rStruct->Owning = false;
	}
	else if (Flags & CASTCLASS_FArrayProperty)
	{
		luaL_argexpected(L, lua_istable(L, Index), Index, "array");
		FArrayProperty* ArrayProperty = Cast<FArrayProperty>(Property);
		FScriptArray* ArrayValue = ArrayProperty->ContainerPtrToValuePtr<FScriptArray>(Container);

		int InputLen = luaL_len(L, Index);
		int ElementSize = ArrayProperty->Inner->ElementSize;
		ArrayValue->Empty(InputLen, ElementSize);
		ArrayValue->AddZeroed(InputLen, ElementSize);

		uint8* Data = static_cast<uint8*>(ArrayValue->GetData());
		for (int i = 0; i < InputLen; ++i)
		{
			lua_pushinteger(L, i + 1);
			lua_gettable(L, Index);
			LuaToProperty(L, ArrayProperty->Inner, Data + ElementSize * i, Index + 1);
			lua_pop(L, 1);
		}
	}
	else
	{
		luaL_error(L, "Property type not supported. Please report this to Feyko");
	}
}


int Lua_GetClass(lua_State* L)
{
	LOG("Getting a class");
	FString ClassName = luaL_checkstring(L, 1);
	FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
	UClass* Class = FTIReflection::FindClassByName(ClassName, Package);
	FLuaUClass::ConstructClass(L, Class);
	return 1;
}

int Lua_MakeStructInstance(lua_State* L)
{
	LOG("Making a struct instance")
	UStruct* BaseStruct;
	if (lua_isuserdata(L, 1))
	{
		BaseStruct = FLuaUStruct::Get(L)->Struct;
	}
	else
	{
		FString StructName = luaL_checkstring(L, 1);
		FString Package = lua_isstring(L, 2) ? lua_tostring(L, 2) : "FactoryGame";
		BaseStruct = FTIReflection::FindStructByName(StructName, Package);
		if (!BaseStruct)
		{
			LOGF("Couldn't find a struct with the name %s", *StructName)
			lua_pushnil(L);
			return 1;
		}
	}
	if (!BaseStruct->IsValidLowLevel())
	{
		LOG("Trying to make an instance of an invalid struct")
		lua_pushnil(L);
		return 1;
	}
	void* Instance = FTIReflection::MakeStructInstance(BaseStruct);
	FLuaUStruct::ConstructStruct(L, BaseStruct, Instance, true);
	return 1;
}

int Lua_MakeSubclass(lua_State* L)
{
	UClass* ParentClass = FLuaUClass::Get(L)->Class;
	FString Name = luaL_checkstring(L, 2);
	UClass* GeneratedClass = FTIReflection::GenerateUniqueSimpleClass(*("/TweakIt/Generated/" + Name), *Name,ParentClass);
	FLuaUClass::ConstructClass(L, GeneratedClass);
	return 1;
}

int Lua_UnlockRecipe(lua_State* L)
{
	UClass* Class = FLuaUClass::Get(L)->Class;
	if (!lua_isuserdata(L, 2))
	{
		lua_getglobal(L, "WorldContext");
	}
	UObject* WorldContext = FLuaUObject::Get(L, 2)->Object;
	FTIContentRegistration::UnlockRecipe(Class, WorldContext);
	return 0;
}

int Lua_LoadObject(lua_State* L)
{
	LOG("Loading an object")
	FString Path = lua_tostring(L, 1);
	UClass* Class = lua_isuserdata(L, 2) ? FLuaUClass::Get(L, 2)->Class : UObject::StaticClass();
	UObject* Object = StaticLoadObject(Class, nullptr, *Path);
	FLuaUObject::ConstructObject(L, Object);
	return 1;
}

int Lua_Print(lua_State* L)
{
	FString String = luaL_checkstring(L, 1);
	LOG(String)
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

int Lua_WaitForEvent(lua_State* L)
{
	FString Event = luaL_checkstring(L, 1);
	FLuaLifecycleNotifier* Notifier = FLuaLifecycleNotifier::Get(L);
	Notifier->WaitForEvent(Event);
	return 0;
}

int Lua_DumpFunction(lua_State* L)
{
	FString Name = luaL_checkstring(L, 1);
	LOGF("Dumping %s", *Name)
	FTILuaFuncManager::DumpFunction(L, Name, 2);
	LOG("Dump finished!")
	return 0;
}

int Lua_LoadFunction(lua_State* L)
{
	FString Name = luaL_checkstring(L, 1);
	LOGF("Loading %s", *Name)
	FLuaFunc* Func = FTILuaFuncManager::Funcs.Find(Name);
	LOGF("Found %s", *Name)
	if (!Func)
	{
		luaL_error(L, "Function %s not previously dumped", TCHAR_TO_UTF8(*Name));
	}
	FTILuaFuncManager::LoadFunction(L, *Func, Name);
	LOG(luaL_typename(L, -1))
	lua_setglobal(L, TCHAR_TO_UTF8(*Name));
	LOG("Load finished!")
	return 0;
}
