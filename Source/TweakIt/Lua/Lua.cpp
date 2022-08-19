#include "Lua.h"
#include <string>
#include "CoreMinimal.h"
#include "FGBlueprintFunctionLibrary.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "FTILuaFuncManager.h"
#include "IPlatformFilePak.h"
#include "LuaState.h"
#include "Scripting/TIScriptOrchestrator.h"
#include "TweakIt/TweakItTesting.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Helpers/TIContentRegistration.h"

using namespace std;

void FTILua::LuaT_ExpectLuaFunction(lua_State* L, int Index)
{
	luaL_argexpected(L, lua_isfunction(L, Index) && !lua_iscfunction(L, Index), Index, "Lua Function");
}

template<typename T>
T* FTILua::LuaT_CheckLightUserdata(lua_State* L, int Index)
{
	luaL_argexpected(L, lua_isuserdata(L, Index), Index, "light userdata");
	return static_cast<T*>(lua_touserdata(L, Index));
}

bool FTILua::LuaT_CheckBoolean(lua_State* L, int Index)
{
	luaL_argexpected(L, lua_isboolean(L, Index), Index, "boolean");
	return static_cast<bool>(lua_toboolean(L, Index));
}

FString FTILua::LuaT_CheckStringable(lua_State* L, int Index)
{
	const char* str = luaL_tolstring(L, Index, nullptr);
	luaL_argexpected(L, str, Index, "string or stringable");
	lua_pop(L, 1);
	return FString(str);
}

bool FTILua::LuaT_OptBoolean(lua_State* L, int Index, bool Default)
{
	return lua_isboolean(L, Index) ? static_cast<bool>(lua_toboolean(L, Index)) : Default;
}

void FTILua::RegisterMetatable(lua_State* L, const char* Name, TArray<luaL_Reg> Regs)
{
	luaL_newmetatable(L, Name);
	for (auto Reg : Regs)
	{
		RegisterMethod(L, Reg);
	}
}

void FTILua::RegisterMethod(lua_State* L, luaL_Reg Reg)
{
	lua_pushstring(L, Reg.name);
	lua_pushcfunction(L, Reg.func);
	lua_settable(L, -3);
}

bool FTILua::CheckLua(lua_State* L, int Returned)
{
	if (Returned != LUA_OK)
	{
		string ErrorMsg = lua_tostring(L, -1);
		LOG(ErrorMsg)
		return false;
	}
	return true;
}

void FTILua::StackDump(lua_State* L)
{
	int Top = lua_gettop(L);
	FString String = "";
	int BiggestName = 3;
	TArray<FString> TypeNames;
	for (int i = 1; i <= Top; i++)
	{
		FString TypeName;
		if (lua_isuserdata(L, i))
		{
			lua_getmetatable(L, i);
			lua_getfield(L, -1, "__name");
			TypeName = luaL_checkstring(L, -1);
			lua_pop(L, 2);
		} else
		{
			TypeName = lua_typename(L, lua_type(L, i));
		}
		if (TypeName.Len() > BiggestName)
		{
			BiggestName = TypeName.Len();
		}
		TypeNames.Emplace(TypeName);
	}
	FString Separator = "<" + FString::ChrN(BiggestName-2, '-') + ">";
	LOG(Separator)
	for (auto TypeName : TypeNames)
	{
		LOG(TypeName)
	}
	LOG(Separator)
}

int FTILua::CallUFunction(lua_State* L, UObject* Object, UFunction* Function, int StartIndex)
{
	LOG("Calling UFunction")
	check(Function->IsValidLowLevel())
	check(Object->IsValidLowLevel())
	void* Params = FMemory_Alloca(Function->ParmsSize * 8);
	LOG(Function->ParmsSize)
	LOG(sizeof FString)
	// void* Params = FMemory::Malloc(Function->ParmsSize * 1);
	// LOG(FMemory::GetAllocSize(Params))
	PopulateUFunctionParams(L, Function, Params, StartIndex);
	LOG("Invoking")
	Function->ProcessEvent(Function, Params);
	FProperty* ReturnProperty = Function->GetReturnProperty();
	LOG("Returned")
	if (ReturnProperty)
	{
		LOG("Returning value to Lua")
		PropertyToLua(L, ReturnProperty, Params);
	}
	LOG("Freeing")
	// FMemory::Free(Params);
	LOG("Done")
	return ReturnProperty->IsValidLowLevel();
}

void FTILua::UFunctionToLua(lua_State* L, UFunction* Function, UObject* Object)
{
	checkf(Object->IsValidLowLevel(), TEXT("Invalid UObject passed for LuaUFunction creation"))
	FLuaUFunction::Construct(L, Function, Object);
}

void FTILua::PopulateUFunctionParams(lua_State* L, UFunction* Function, void* Params, int StartIndex) {
	int i = StartIndex;
	for (FProperty* Prop = Function->PropertyLink; Prop; Prop = Prop->PropertyLinkNext)
	{
		if (Prop->HasAnyPropertyFlags(CPF_ReturnParm) || !Prop->HasAllPropertyFlags(CPF_Parm))
		{
			continue;
		}
		LuaToProperty(L, Prop, Params, i);
		// LOG(Prop->ContainerPtrToValuePtr<void>(Params))
		// FString uhm = CastFieldChecked<FStrProperty>(Prop)->GetPropertyValue_InContainer(Params);
		LOG(**CastFieldChecked<FStrProperty>(Prop)->GetPropertyValuePtr_InContainer(Params))
		// LOG(static_cast<void*>(const_cast<wchar_t*>(*uhm)))
		i++;
	}
	
}

// Mostly borrowed from FIN's source. Thanks Pana !
void FTILua::PropertyToLua(lua_State* L, FProperty* Property, void* Container, bool Local /*= false*/)
{
	LOGF("Transforming from Property %s to Lua", *Property->GetName());
	if (Local)
	{
		LOG("Localising property container")
		Container = static_cast<uint8*>(Container) - Property->GetOffset_ForDebug();
	}
	if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
	{
		lua_pushboolean(L, BoolProp->GetPropertyValue_InContainer(Container));
	}
	else if (FInt8Property* Int8Prop = CastField<FInt8Property>(Property))
	{
		lua_pushinteger(L, Int8Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FInt16Property* Int16Prop = CastField<FInt16Property>(Property))
	{
		lua_pushinteger(L, Int16Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FIntProperty* IntProp = CastField<FIntProperty>(Property))
	{
		lua_pushinteger(L, IntProp->GetPropertyValue_InContainer(Container));
	}
	else if (FInt64Property* Int64Prop = CastField<FInt64Property>(Property))
	{
		lua_pushinteger(L, Int64Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FUInt16Property* UInt16Prop = CastField<FUInt16Property>(Property))
	{
		lua_pushinteger(L, UInt16Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FUInt32Property* UInt32Prop = CastField<FUInt32Property>(Property))
	{
		lua_pushinteger(L, UInt32Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FUInt64Property* UInt64Prop = CastField<FUInt64Property>(Property))
	{
		lua_pushinteger(L, UInt64Prop->GetPropertyValue_InContainer(Container));
	}
	else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
	{
		lua_pushnumber(L, FloatProp->GetPropertyValue_InContainer(Container));
	}
	else if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Property))
	{
		lua_pushnumber(L, DoubleProp->GetPropertyValue_InContainer(Container));
	}
	else if (FStrProperty* StrProp = CastField<FStrProperty>(Property))
	{
		FString String = StrProp->GetPropertyValue_InContainer(Container);
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (FNameProperty* NameProp = CastField<FNameProperty>(Property))
	{
		FString String = NameProp->ContainerPtrToValuePtr<FName>(Container)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (FTextProperty* TextProp = CastField<FTextProperty>(Property))
	{
		FString String = TextProp->ContainerPtrToValuePtr<FText>(Container)->ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*String));
	}
	else if (FClassProperty* ClassProp = CastField<FClassProperty>(Property))
	{
		FLuaUClass::ConstructClass(L, *ClassProp->ContainerPtrToValuePtr<UClass*>(Container));
	}
	else if (FEnumProperty* EnumProp = CastField<FEnumProperty>(Property))
	{
		int64 EnumValue = *EnumProp->ContainerPtrToValuePtr<uint8>(Container);
		UEnum* Enum = EnumProp->GetEnum();
		if (!Enum->IsValidEnumValue(EnumValue))
		{
			luaL_error(L, "Enum value wasn't valid. Please report this to Feyko");
		}
		FName Name = Enum->GetNameByValue(EnumValue);
		FString StringName = Name.ToString();
		lua_pushstring(L, TCHAR_TO_UTF8(*StringName));
	}
	else if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
	{
		void* StructValue = StructProp->ContainerPtrToValuePtr<void>(Container);
		FLuaUStruct::ConstructStruct(L, StructProp->Struct, StructValue);
	}
	else if (FObjectProperty* ObjectProp = CastField<FObjectProperty>(Property))
	{
		FLuaUObject::ConstructObject(L, ObjectProp->GetPropertyValue_InContainer(Container));
	}
	else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
	{
		FLuaTArray::ConstructArray(L, ArrayProp, Container);
	}
	else if (FDelegateProperty* DelegateProp = CastField<FDelegateProperty>(Property))
	{
		FScriptDelegate* Value = DelegateProp->ContainerPtrToValuePtr<FScriptDelegate>(Container);
		FLuaFDelegate::Construct(L, DelegateProp->SignatureFunction, Value);
	}
	else if (FInterfaceProperty* InterfaceProp = CastField<FInterfaceProperty>(Property))
	{
		FScriptInterface* Interface = InterfaceProp->ContainerPtrToValuePtr<FScriptInterface>(Container);
		FLuaUObject::ConstructObject(L, Interface->GetObject());
	}
	else if (FFieldPathProperty* FieldPathProp = CastField<FFieldPathProperty>(Property))
	{
		lua_pushstring(L, TCHAR_TO_UTF8(*FieldPathProp->GetPropertyValuePtr_InContainer(Container)->ToString()));
	}
	else
	{
		FString Error = FString::Printf(TEXT("Property type %s is unsupported. Please report this to Feyko"), *Property->GetCPPType());
		luaL_error(L, TCHAR_TO_UTF8(*Error));
	}
}

// Mostly borrowed from FIN's source. Thanks Pana !
void FTILua::LuaToProperty(lua_State* L, FProperty* Property, void* Container, int Index, bool Local /*= false*/)
{
	LOGF("Transforming from Lua to Property %s", *Property->GetName());
	if (Local)
	{
		LOG("Localising property container")
		Container = static_cast<uint8*>(Container) - Property->GetOffset_ForDebug();
	}
	if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
	{
		luaL_argexpected(L, lua_isboolean(L, Index), Index, "boolean");
		BoolProp->SetPropertyValue_InContainer(Container, LuaT_CheckBoolean(L, Index));
	}
	else if (FInt8Property* Int8Prop = CastField<FInt8Property>(Property))
	{
		Int8Prop->SetPropertyValue_InContainer(Container, luaL_checkinteger(L, Index));
	}
	else if (FInt16Property* Int16Prop = CastField<FInt16Property>(Property))
	{
		Int16Prop->SetPropertyValue_InContainer(Container, luaL_checkinteger(L, Index));
	}
	else if (FIntProperty* IntProp = CastField<FIntProperty>(Property))
	{
		int Int = luaL_checkinteger(L, Index);
		IntProp->SetPropertyValue_InContainer(Container, Int);
	}
	else if (FInt64Property* Int64Prop = CastField<FInt64Property>(Property))
	{
		*Int64Prop->ContainerPtrToValuePtr<std::int64_t>(Container) = luaL_checkinteger(L, Index);
	}
	else if (FUInt16Property* UInt16Prop = CastField<FUInt16Property>(Property))
	{
		UInt16Prop->SetPropertyValue_InContainer(Container, luaL_checkinteger(L, Index));
	}
	else if (FUInt32Property* UInt32Prop = CastField<FUInt32Property>(Property))
	{
		UInt32Prop->SetPropertyValue_InContainer(Container, luaL_checkinteger(L, Index));
	}
	else if (FUInt64Property* UInt64Prop = CastField<FUInt64Property>(Property))
	{
		UInt64Prop->SetPropertyValue_InContainer(Container, luaL_checkinteger(L, Index));
	}
	else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
	{
		FloatProp->SetPropertyValue_InContainer(Container, luaL_checknumber(L, Index));
	}
	else if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Property))
	{
		DoubleProp->SetPropertyValue_InContainer(Container, luaL_checknumber(L, Index));
	}
	else if (FStrProperty* StrProp = CastField<FStrProperty>(Property))
	{
		FString String = luaL_checkstring(L, Index);
		// LOG(static_cast<void*>(const_cast<wchar_t*>(*String)))
		// LOG(String)
		LOG(*String)
		StrProp->SetPropertyValue_InContainer(Container, String);
		// *StrProp->GetPropertyValuePtr_InContainer(Container) = String;
		LOG(**StrProp->GetPropertyValuePtr_InContainer(Container))
		// LOG(static_cast<void*>(const_cast<wchar_t*>(StrProp->GetPropertyValuePtr_InContainer(Container)->GetCharArray().GetData())))
		// LOG(GetData(*StrProp->GetPropertyValuePtr_InContainer(Container)))
		// LOG(StrProp->GetPropertyValue_InContainer(Container))
		// LOG(*StrProp->GetPropertyValuePtr_InContainer(Container))
		// LOG(*StrProp->GetPropertyValue_InContainer(Container))
		// LOG(**StrProp->GetPropertyValuePtr_InContainer(Container))
	}
	else if (FNameProperty* NameProp = CastField<FNameProperty>(Property))
	{
		NameProp->SetPropertyValue_InContainer(Container, luaL_checkstring(L, Index));
	}
	else if (FTextProperty* TextProp = CastField<FTextProperty>(Property))
	{
		TextProp->SetPropertyValue_InContainer(Container, FText::FromString(luaL_checkstring(L, Index)));
	}
	else if (FClassProperty* ClassProp = CastField<FClassProperty>(Property))
	{
		FLuaUClass* LuaUClass = FLuaUClass::Get(L, Index);
		ClassProp->SetPropertyValue_InContainer(Container, LuaUClass->Class);
	}
	else if (FEnumProperty* EnumProp = CastField<FEnumProperty>(Property))
	{
		FName EnumValueName = luaL_checkstring(L, Index);
		FString EnumValueString = EnumValueName.ToString();
		UEnum* Enum = EnumProp->GetEnum();
		if (!EnumValueString.StartsWith(Enum->GetName()))
		{
			EnumValueString = Enum->GetName() + "::" + EnumValueString;
			EnumValueName = FName(EnumValueString);
		}
		if (!Enum->IsValidEnumName(EnumValueName))
		{
			luaL_error(L, "invalid enum value %s for enum type %s. Example value: %s",
			           TCHAR_TO_UTF8(*EnumValueName.ToString()), TCHAR_TO_UTF8(*Enum->GetName()),
			           TCHAR_TO_UTF8(*Enum->GetNameByIndex(0).ToString()));
			return;
		}
		int64 EnumValue = Enum->GetValueByName(EnumValueName);
		*EnumProp->ContainerPtrToValuePtr<uint8>(Container) = static_cast<uint8>(EnumValue);
	}
	else if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
	{
		FLuaUStruct* rStruct = FLuaUStruct::Get(L, Index);
		if (!(StructProp->Struct->GetFullName() == rStruct->Struct->GetFullName()))
		{
			luaL_error(L, "Mismatched struct types (%s <- %s)",
			           TCHAR_TO_UTF8(*StructProp->Struct->GetName()), TCHAR_TO_UTF8(*rStruct->Struct->GetName()));
			return;
		}
		StructProp->CopyCompleteValue_InContainer(Container, rStruct->Values);
		void* NewValue = StructProp->ContainerPtrToValuePtr<void>(Container);
		rStruct->Values = NewValue;
	}
	else if (FObjectProperty* ObjectProp = CastField<FObjectProperty>(Property))
	{
		UObject* Object = lua_isnil(L, Index) ? nullptr : FLuaUObject::Get(L, Index)->Object;
		ObjectProp->SetPropertyValue_InContainer(Container, Object);
	}
	else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
	{
		luaL_argexpected(L, lua_istable(L, Index), Index, "array");
		FScriptArray* ArrayValue = ArrayProp->ContainerPtrToValuePtr<FScriptArray>(Container);

		int InputLen = luaL_len(L, Index);
		int ElementSize = ArrayProp->Inner->ElementSize;
		ArrayValue->Empty(InputLen, ElementSize);
		ArrayValue->AddZeroed(InputLen, ElementSize);

		uint8* Data = static_cast<uint8*>(ArrayValue->GetData());
		for (int i = 0; i < InputLen; ++i)
		{
			lua_pushinteger(L, i + 1);
			lua_gettable(L, Index);
			LuaToProperty(L, ArrayProp->Inner, Data + ElementSize * i, Index + 1);
			lua_pop(L, 1);
		}
	}
	else if (FDelegateProperty* DelegateProp = CastField<FDelegateProperty>(Property))
	{
		luaL_error(L, "Delegate assignment is not yet supported");
	}
	else if (FInterfaceProperty* InterfaceProp = CastField<FInterfaceProperty>(Property))
	{
		UObject* Object = lua_isnil(L, Index) ? nullptr : FLuaUObject::Get(L, Index)->Object;
		FScriptInterface* InterfacePtr = InterfaceProp->ContainerPtrToValuePtr<FScriptInterface>(Container);
		void* ObjectInterface = nullptr;
		if(Object)
		{
			UClass* InterfaceClass = InterfaceProp->InterfaceClass;
			ObjectInterface = Object->GetInterfaceAddress(InterfaceClass);
			if (!ObjectInterface)
			{
				FString Error = FString::Printf(
					TEXT("Tried to assign object %s to interface of type %s but it does not implement said interface."),
					*Object->GetName(), *InterfaceClass->GetName());
				luaL_error(L, TCHAR_TO_UTF8(*Error));
				return;
			}
		}
		*InterfacePtr = FScriptInterface(Object, ObjectInterface);
	}
	else if (FFieldPathProperty* FieldPathProp = CastField<FFieldPathProperty>(Property))
	{
		FString PathString = luaL_checkstring(L, Index);
		FFieldPath Path = FFieldPath();
		Path.Generate(*PathString);
		FieldPathProp->SetPropertyValue_InContainer(Container, Path);
	}
	else
	{
		FString Error = FString::Printf(TEXT("Property type %s is unsupported. Please report this to Feyko"), *Property->GetCPPType());
		luaL_error(L, TCHAR_TO_UTF8(*Error));
	}
}


int FTILua::Lua_GetClass(lua_State* L)
{
	LOG("Getting a class");
	FString ClassName = luaL_checkstring(L, 1);
	FString Package = luaL_optstring(L, 2, "FactoryGame");
	UClass* Class = FTIReflection::FindClassByName(ClassName, Package);
	FLuaUClass::ConstructClass(L, Class);
	return 1;
}

int FTILua::Lua_MakeStructInstance(lua_State* L)
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
		FString Package = luaL_optstring(L, 2, "FactoryGame");
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

int FTILua::Lua_MakeSubclass(lua_State* L)
{
	UClass* ParentClass = FLuaUClass::Get(L)->Class;
	FString Name = luaL_checkstring(L, 2);
	UClass* GeneratedClass = FTIReflection::GenerateUniqueSimpleClass(*("/TweakIt/Generated/" + Name), *Name,ParentClass);
	FLuaUClass::ConstructClass(L, GeneratedClass);
	return 1;
}

int FTILua::Lua_UnlockRecipe(lua_State* L)
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

int FTILua::Lua_LoadObject(lua_State* L)
{
	LOG("Loading an object")
	FString Path = luaL_checkstring(L, 1);
	UClass* Class = lua_isuserdata(L, 2) ? FLuaUClass::Get(L, 2)->Class : UObject::StaticClass();
	UObject* Object = StaticLoadObject(Class, nullptr, *Path);
	FLuaUObject::ConstructObject(L, Object);
	return 1;
}

int FTILua::Lua_Print(lua_State* L)
{
	int Num = lua_gettop(L);
	FString Out;
	for (int i = 0; i < Num; ++i)
	{
		Out += LuaT_CheckStringable(L, i) + " ";
	}
	LOG(Out.TrimEnd())
	return 0;
}

int Ayo(lua_State* L)
{
	LOG("AYOOOO")
	return 0;
}

int FTILua::Lua_Test(lua_State* L)
{
	LOG("Running Lua_Test")
	// LuaT_ExpectLuaFunction(L, 1);
	luaL_checkinteger(L, 1);
	RegisterMetatable(L, "custom", {
		{"__index", Ayo}
	});
	StackDump(L);
	// luaL_setmetatable(L, "custom");
	StackDump(L);
	int r = lua_getmetatable(L, 1);
	LOG(r)
	StackDump(L);
	return 0;
}

int FTILua::Lua_WaitForEvent(lua_State* L)
{
	FString Event = luaL_checkstring(L, 1);
	bool Unique = LuaT_OptBoolean(L, 2, true);
	if (Unique && FTIScriptOrchestrator::Get()->HasEventPassed(Event))
	{
		return 0;
	}
	FLuaState* State = FLuaState::Get(L);
	State->EventWaitedFor = Event;
	lua_yield(L, 0);
	return 0;
}

int FTILua::Lua_WaitForMod(lua_State* L)
{
	FString Event = luaL_checkstring(L, 1);
	FString Lifecycle = luaL_optstring(L, 2, "Module");
	lua_settop(L, 0);
	lua_pushstring(L, TCHAR_TO_UTF8(*FTIScriptOrchestrator::MakeEventForMod(Event, Lifecycle)));
	Lua_WaitForEvent(L);
	return 0;
}

int FTILua::Lua_DumpFunction(lua_State* L)
{
	FString Name = luaL_checkstring(L, 1);
	FTILuaFuncManager::DumpFunction(L, Name, 2);
	return 0;
}

int FTILua::Lua_LoadFunction(lua_State* L)
{
	FString Name = luaL_checkstring(L, 1);
	TResult<FLuaFunc> Func = FTILuaFuncManager::GetSavedLuaFunc(Name);
	if (!Func)
	{
		luaL_error(L, "Function %s not previously dumped", TCHAR_TO_UTF8(*Name));
	}
	FTILuaFuncManager::LoadFunction(L, *Func, Name);
	lua_setglobal(L, TCHAR_TO_UTF8(*Name));
	return 1;
}
