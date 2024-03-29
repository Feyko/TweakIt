#pragma once
#include "Lua.h"

class FLuaState
{
public:
	FLuaState();
	~FLuaState();

	void RegisterWorldContext(UObject* Context);
	static FLuaState* Get(lua_State* L);

	FString EventWaitedFor;
	FEvent* PlatformEventWaitedFor;
	
	lua_State* L;
private:
	void OpenLibs();
	void RegisterMetadatas();
	void RegisterGlobalFunctions();

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
		{"WaitForMod", FTILua::Lua_WaitForMod},
		{"DumpFunction", FTILua::Lua_DumpFunction},
		{"LoadFunction", FTILua::Lua_LoadFunction}
	};
};
