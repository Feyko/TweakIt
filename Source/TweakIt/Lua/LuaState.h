#pragma once
#include "Lua.h"
#include "LuaLifecycleNotifier.h"

#include "TweakIt/Lua/Lua.h"

class FLuaState
{
public:
	explicit FLuaState(UObject* WorldContext);
	lua_State* L;
	FLuaLifecycleNotifier* LifecycleNotifier;
private:
	void OpenLibs();
	void RegisterMetadatas();
	void RegisterGlobalFunctions();
	void RegisterWorldContext(UObject* Context);
	void RegisterLifecycleNotifier();

	inline static TArray<luaL_Reg> GlobalFunctions = {
		{"GetClass", Lua_GetClass},
		{"LoadObject", Lua_LoadObject},
		{"UnlockRecipe", Lua_UnlockRecipe},
		{"Log", Lua_Print},
		{"print", Lua_Print},
		{"MakeSubclass", Lua_MakeSubclass},
		{"MakeStructInstance", Lua_MakeStructInstance},
		{"Test", Lua_Test},
		{"WaitForEvent", Lua_WaitForEvent},
		{"DumpFunction", Lua_DumpFunction},
		{"LoadFunction", Lua_LoadFunction}
	};
};
