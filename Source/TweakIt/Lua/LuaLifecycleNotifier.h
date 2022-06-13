#pragma once
#include "Lua.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEventDelegate, FString)
DECLARE_MULTICAST_DELEGATE(FScriptWaitingDelegate)
DECLARE_MULTICAST_DELEGATE(FScriptResumeDelegate)

class FLuaLifecycleNotifier
{
public:
	FLuaLifecycleNotifier();
	static int Construct(lua_State* L);
	static FLuaLifecycleNotifier* Get(lua_State* L);

	void BroadcastEvent(FString Event);
	void WaitForEvent(FString WantedEvent);
	
	static int Lua_WaitForEvent(lua_State* L);

	static int Lua__index(lua_State* L);
	
	FEventDelegate EventDelegate;
	FScriptWaitingDelegate ScriptWaitingDelegate;
	FScriptResumeDelegate ScriptResumeDelegate;

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "LifecycleNotifier";
	
private:
	void SetupHooks();
	
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		{"WaitForEvent", Lua_WaitForEvent},
	};
};
