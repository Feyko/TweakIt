#pragma once
#include "Lua.h"

#include "TweakIt\Lua\Lua.h"

class FLuaState
{
public:
	explicit FLuaState(UObject* WorldContext);
	lua_State* L = nullptr;
private:
	void OpenLibs();
	void RegisterMetadatas();
	void RegisterGlobalFunctions();
	void RegisterWorldContext(UObject* Context);

	inline static TArray<luaL_Reg> GlobalFunctions = {
		{"GetClass", lua_GetClass},
		{"LoadObject", lua_LoadObject},
		{"UnlockRecipe", lua_UnlockRecipe},
		{"Log", lua_Print},
		{"print", lua_Print},
		{"MakeSubclass", lua_MakeSubclass},
		{"MakeStructInstance", lua_MakeStructInstance},
		{"Test", lua_Test},
	};
};
