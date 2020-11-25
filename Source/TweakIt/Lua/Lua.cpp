#include "Lua.h"
#include <string>
#include "CoreMinimal.h"
#include "D:/SatisfactoryModding/SatisfactoryModLoader/Source/TweakIt/LuaLib/lua.hpp"
#include "FGBuildableManufacturer.h"
#include "FGRecipeManager.h"
#include "IPlatformFilePak.h"
#include "LuaUStruct.h"
#include "TIReflection.h"
#include "TweakItGameSubsystem.h"
#include "UObjectIterator.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Lua/LuaUClass.h"
#include "Lua/LuaUObject.h"
#include "util/Logging.h"
#include "Lua/LuaTArray.h"
using namespace std;

namespace TweakIt
{
	namespace Lua
	{
		bool CheckLua(lua_State* L, int r) {
			if (r != LUA_OK) {
				string errormsg = lua_tostring(L, -1);
				LOGS(errormsg)
				return false;
			} else {
				return true;
			}
		}

		void stackDump(lua_State* L) {

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
		void propertyToLua(lua_State* L, UProperty* p, void* data) {
			LOGFS(FString::Printf(TEXT("Transforming from Property %s to Lua"), *p->GetName()));
			auto c = p->GetClass()->ClassCastFlags;
			if (c & EClassCastFlags::CASTCLASS_UBoolProperty) {
				lua_pushboolean(L, *p->ContainerPtrToValuePtr<bool>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UIntProperty) {
				lua_pushinteger(L, *p->ContainerPtrToValuePtr<std::int32_t>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UInt64Property) {
				lua_pushinteger(L, *p->ContainerPtrToValuePtr<std::int64_t>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UFloatProperty) {
				lua_pushnumber(L, *p->ContainerPtrToValuePtr<float>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UStrProperty) {
				FString string = *p->ContainerPtrToValuePtr<FString>(data);
				lua_pushstring(L, TCHAR_TO_UTF8(*string));
			} else if (c & EClassCastFlags::CASTCLASS_UNameProperty) {
				FString NameString = p->ContainerPtrToValuePtr<FName>(data)->ToString();
				lua_pushstring(L, TCHAR_TO_UTF8(*NameString));
			} else if (c & EClassCastFlags::CASTCLASS_UClassProperty) {
				LuaUClass::ConstructClass(L, *p->ContainerPtrToValuePtr<UClass*>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UEnumProperty) {
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
			} else if (c & EClassCastFlags::CASTCLASS_UStructProperty) {
				UStructProperty* StructProperty = Cast<UStructProperty>(p);
				LuaUStruct::ConstructStruct(L, StructProperty, data);
			} else if (c & EClassCastFlags::CASTCLASS_UObjectProperty) {
				LuaUObject::ConstructObject(L, p->ContainerPtrToValuePtr<UObject>(data));
			} else if (c & EClassCastFlags::CASTCLASS_UArrayProperty) {
				UArrayProperty* prop = Cast<UArrayProperty>(p);
				LuaTArray::ConstructArray(L, prop, data);
			} else {
				lua_pushnil(L);
			}
		}

		// Mostly borrowed from FIN's source. Thanks Pana !
		void luaToProperty(lua_State* L, UProperty* p, void* data, int i) {
			LOGFS(FString::Printf(TEXT("Transforming from Lua to Property %s"), *p->GetName()));
			auto c = p->GetClass()->ClassCastFlags;
			if (c & EClassCastFlags::CASTCLASS_UBoolProperty) {
				*p->ContainerPtrToValuePtr<bool>(data) = static_cast<bool>(lua_toboolean(L, i));
			} else if (c & EClassCastFlags::CASTCLASS_UIntProperty) {
				*p->ContainerPtrToValuePtr<std::int32_t>(data) = static_cast<std::int32_t>(lua_tointeger(L, i));
			} else if (c & EClassCastFlags::CASTCLASS_UInt64Property) {
				*p->ContainerPtrToValuePtr<std::int64_t>(data) = static_cast<std::int64_t>(lua_tointeger(L, i));
			} else if (c & EClassCastFlags::CASTCLASS_UFloatProperty) {
				*p->ContainerPtrToValuePtr<float>(data) = static_cast<float>(lua_tonumber(L, i));
			} else if (c & EClassCastFlags::CASTCLASS_UStrProperty) {
				size_t len;
				const char* s = lua_tolstring(L, i, &len);
				if (!s) throw std::exception("Invalid String in string property parse");
				FString* o = p->ContainerPtrToValuePtr<FString>(data);
				*o = FString(UTF8_TO_TCHAR(s));
			} else if (c & EClassCastFlags::CASTCLASS_UClassProperty) {
				UClass* o = ((LuaUClass*)lua_touserdata(L, i))->Class;
				*p->ContainerPtrToValuePtr<UClass*>(data) = o;
			} else if (c & EClassCastFlags::CASTCLASS_UEnumProperty) {
				FName NameEnumValue = lua_tostring(L, i);
				UEnum* Enum = Cast<UEnumProperty>(p)->GetEnum();
				if (Enum->IsValidEnumName(NameEnumValue)) {
					int64 EnumValue = Enum->GetValueByName(NameEnumValue);
					*p->ContainerPtrToValuePtr<uint8>(data) = static_cast<uint8>(EnumValue);
				}
			} else if (c & EClassCastFlags::CASTCLASS_UStructProperty) {
				LOG("Struct assignation isn't supported for now. Please edit the struct's values instead")
			} else if (c & EClassCastFlags::CASTCLASS_UArrayProperty) {
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

		int lua_MakeSubclass(lua_State* L) {
			UClass* ParentClass = static_cast<LuaUClass*>(lua_touserdata(L, 1))->Class;
			FString Name = lua_tostring(L, 2);
			UClass* GeneratedClass = FTIReflection::GenerateSimpleClass(*("/TweakIt/" + Name), *Name, ParentClass);
			LuaUClass::ConstructClass(L, GeneratedClass);
			return 1;
		}

		namespace Registry
		{
			int lua_UnlockRecipe(lua_State* L) {
				LOG("Unlocking a recipe")
				UClass* Class = static_cast<LuaUClass*>(lua_touserdata(L, 1))->Class;
				AFGRecipeManager* Manager = AFGRecipeManager::Get(Class);
				TSubclassOf<UFGRecipe> Recipe = Class;
				if (!Recipe->IsValidLowLevel()) {
					LOG("The class isn't a recipe")
					return 0;
				}
				if (!Manager->IsValidLowLevel()) {
					LOG("The recipe manager isn't valid")
					return 0;
				}
				if (Manager->IsRecipeAvailable(Recipe)) {
					LOG("Recipe is already unlocked")
					return 0;
				}
				Manager->AddAvailableRecipe(Recipe);
				return 0;
			}
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
			if (lua_isstring(L, -1)) {
				string s = lua_tostring(L, -1);
				LOGS(s)
			}
			return 0;
		}
	}
}

using namespace TweakIt::Lua;
