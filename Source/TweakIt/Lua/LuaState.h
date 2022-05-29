#pragma once
#include "lib/lua.hpp"

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
};
