#pragma once

#include "TweakIt/Lua/Lua.h"

struct FLuaUFunction : FGCObject
{
	explicit FLuaUFunction(UFunction* Function, UObject* Object);
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	static int Construct(lua_State* L, UFunction* Function, UObject* Object);
	static FLuaUFunction* Get(lua_State* L, int Index = 1);

	static int Lua_On(lua_State* L);
	static int Lua_Bind(lua_State* L);
	
	static int Lua__index(lua_State* L);
	static int Lua__call(lua_State* L);
	static int Lua__tostring(lua_State* L);
	static int Lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "UFunction";

	UFunction* Function;
	UObject* Object;
	
private:
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__call", Lua__call},
		{"__tostring", Lua__tostring},
		{"__gc",	Lua__gc},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		{"On", Lua_On},
		{"Bind", Lua_Bind}
	};
};
