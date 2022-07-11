#pragma once
#include "Lua.h"

#include "TweakIt/Lua/Lua.h"

class FLuaState
{
public:
	explicit FLuaState(UObject* WorldContext);
	~FLuaState();
	lua_State* L;
private:
	void OpenLibs();
	void RegisterMetadatas();
	void RegisterGlobalFunctions();
	void RegisterWorldContext(UObject* Context);

	inline static TArray<luaL_Reg> GlobalFunctions = {
		{"GetClass", FTILua::Lua_GetClass},
		{"LoadObject", FTILua::Lua_LoadObject},
		{"UnlockRecipe", FTILua::Lua_UnlockRecipe},
		{"Log", FTILua::Lua_Print},
		{"print", FTILua::Lua_Print},
		{"MakeSubclass", FTILua::Lua_MakeSubclass},
		{"MakeStructInstance", FTILua::Lua_MakeStructInstance},
		{"Test", FTILua::Lua_Test},
		{"WaitForEvent", FTILua::Lua_WaitForEvent},
		{"DumpFunction", FTILua::Lua_DumpFunction},
		{"LoadFunction", FTILua::Lua_LoadFunction}
	};
};
