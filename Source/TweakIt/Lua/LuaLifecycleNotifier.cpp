#include "LuaLifecycleNotifier.h"

#include "TweakIt/TweakItModule.h"

int FLuaLifecycleNotifier::Construct(lua_State* L)
{
	LOG("Constructing a Lifecycle Notifier")
	FLuaLifecycleNotifier* ReturnedInstance = static_cast<FLuaLifecycleNotifier*>(lua_newuserdata(L, sizeof(FLuaLifecycleNotifier)));
	new(ReturnedInstance) FLuaLifecycleNotifier();
	luaL_getmetatable(L, Name);
	lua_setmetatable(L, -2);
	lua_setglobal(L, Name);
	return 1;
}

FLuaLifecycleNotifier* FLuaLifecycleNotifier::Get(lua_State* L)
{
	lua_getglobal(L, Name);
	if (lua_isnil(L, -1))
	{
		luaL_error(L, "Could not find the Lifecycle Notifier in the global state");
		return nullptr;
	}
	return static_cast<FLuaLifecycleNotifier*>(luaL_checkudata(L, -1, Name));
}

void FLuaLifecycleNotifier::WaitForEvent(FString WantedEvent)
{
	bool Done = false;
	EventDelegate.AddLambda([WantedEvent, &Done](FString Event)
	{
		if (Event == WantedEvent)
		{
			Done = true;
		}
	});
	ScriptWaitingDelegate.Broadcast();
	while (!Done)
	{
		FPlatformProcess::Sleep(0);
	}
	ScriptResumeDelegate.Broadcast();
}

int FLuaLifecycleNotifier::Lua_WaitForEvent(lua_State* L)
{
	FLuaLifecycleNotifier* Self = Get(L);
	FString Event = luaL_checkstring(L, 2);
	Self->WaitForEvent(Event);
	return 0;
}

int FLuaLifecycleNotifier::Lua__index(lua_State* L)
{
	FString Index = luaL_checkstring(L, 2);
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

void FLuaLifecycleNotifier::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}