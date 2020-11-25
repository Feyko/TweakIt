#pragma once

#include "CoreMinimal.h"
#include "LuaLib/lua.hpp"
#include "util/Logging.h"
#include "LuaUClass.h"
#include "LuaUObject.h"

#define LOG(x) UE_LOG(LogTemp, Log, TEXT(x)); SML::Logging::warning(TEXT(x));
#define LOGC(x, level) UE_LOG(LogTemp, level, TEXT(x)); SML::Logging::warning(TEXT(x));
#define LOGS(x) UE_LOG(LogTemp, Log, TEXT("%s"), *FString(x.c_str())); SML::Logging::warning(*FString(x.c_str()));
#define LOGFS(x) UE_LOG(LogTemp, Log, TEXT("%s"), *x); SML::Logging::warning(*x);
#define LOGF(x, ...) LOGFS(FString::Printf(TEXT(x), __VA_ARGS__))

namespace TweakIt
{
	namespace Lua
	{
		bool CheckLua(lua_State* L, int r);

		void stackDump(lua_State* L);

		// Mostly borrowed from FIN's source. Thanks Pana !
		void propertyToLua(lua_State* L, UProperty* p, void* data);

		// Mostly borrowed from FIN's source. Thanks Pana !
		void luaToProperty(lua_State* L, UProperty* p, void* data, int i);

		int lua_GetClass(lua_State* L);

		int lua_MakeSubclass(lua_State* L);

		namespace Registry
		{
			int lua_UnlockRecipe(lua_State* L);
		}

		int lua_LoadObject(lua_State* L);

		int lua_Print(lua_State* L);
	}
}
