#include "FLuaState.h"

#include "Lua.h"
#include "LuaTArray.h"
#include "LuaUClass.h"
#include "LuaUObject.h"
#include "LuaUStruct.h"
#include "TweakIt/TweakItModule.h"

FLuaState::FLuaState(UObject* WorldContext) {
	L = luaL_newstate();
	OpenLibs();
	RegisterMetadatas();
	RegisterGlobalFunctions();
	RegisterWorldContext(WorldContext);
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
	LuaUClass::RegisterMetadata(L);
	LuaUObject::RegisterMetadata(L);
	LuaTArray::RegisterMetadata(L);
	LuaUStruct::RegisterMetadata(L);
}

void FLuaState::RegisterGlobalFunctions() {
	lua_register(L, "GetClass", lua_GetClass);
	lua_register(L, "LoadObject", lua_LoadObject);
	lua_register(L, "UnlockRecipe", lua_UnlockRecipe);
	lua_register(L, "Log", lua_Print);
	lua_register(L, "print", lua_Print);
	lua_register(L, "MakeSubclass", lua_MakeSubclass);
	lua_register(L, "MakeStructInstance", lua_MakeStructInstance);
}

void FLuaState::RegisterWorldContext(UObject* Context)
{
	LuaUObject::ConstructObject(L, Context);
	lua_setglobal(L, "WorldContext");
}