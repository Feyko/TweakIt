#pragma once

#include "TweakIt/Lua/lib/lua.hpp"

struct FLuaTArray : FGCObject
{
	FLuaTArray(FArrayProperty* Property, void* Container);
	
	FArrayProperty* ArrayProperty;
	void* Container;

	static int ConstructArray(lua_State* L, FArrayProperty* ArrayProperty, void* Container);
	static FLuaTArray* Get(lua_State* L, int Index = 1);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	static int Lua__index(lua_State* L);
	static int Lua__newindex(lua_State* L);
	static int Lua__tostring(lua_State* L);
	static int Lua__len(lua_State* L);
	static int Lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "TArray";

private:
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__newindex", Lua__newindex},
		{"__tostring", Lua__tostring},
		{"__len", Lua__len},
		{"__gc", Lua__gc}
	};
};
