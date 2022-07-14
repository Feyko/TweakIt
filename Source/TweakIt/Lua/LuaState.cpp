#include "LuaState.h"

#include "TweakIt/Logging/FTILog.h"

FLuaState::FLuaState()
{
	L = luaL_newstate();
	OpenLibs();
	RegisterMetadatas();
	RegisterGlobalFunctions();
	lua_pushlightuserdata(L, this);
	lua_setfield(L, LUA_REGISTRYINDEX, "State");
}

FLuaState::~FLuaState()
{
	lua_close(L);
}

void FLuaState::OpenLibs()
{
	luaL_Reg Libs[] = {
		{"_G", luaopen_base},
		{LUA_LOADLIBNAME, luaopen_package},
		{LUA_COLIBNAME, luaopen_coroutine},
		{LUA_TABLIBNAME, luaopen_table},
		{LUA_STRLIBNAME, luaopen_string},
		{LUA_MATHLIBNAME, luaopen_math},
		{LUA_UTF8LIBNAME, luaopen_utf8},
		{LUA_DBLIBNAME, luaopen_debug},
	};
	for (auto Lib : Libs)
	{
		luaL_requiref(L, Lib.name, Lib.func, 1);
		lua_pop(L, 1);
	}
}

void FLuaState::RegisterMetadatas()
{
	FLuaUClass::RegisterMetadata(L);
	FLuaUObject::RegisterMetadata(L);
	FLuaTArray::RegisterMetadata(L);
	FLuaUStruct::RegisterMetadata(L);
	FLuaFDelegate::RegisterMetadata(L);
}

void FLuaState::RegisterGlobalFunctions()
{
	for (auto Function : GlobalFunctions)
	{
		lua_register(L, Function.name, Function.func);
	}
}

void FLuaState::RegisterWorldContext(UObject* Context)
{
	FLuaUObject::ConstructObject(L, Context);
	lua_setglobal(L, "WorldContext");
}
