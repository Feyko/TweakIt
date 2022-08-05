#pragma once
#include "CoreMinimal.h"

#include "TweakIt/Lua/Lua.h"

struct FLuaFMulticastDelegate : FGCObject
{
	FLuaFMulticastDelegate(UFunction* Signature, FMulticastScriptDelegate* Delegate);
	
	UFunction* SignatureFunction;
	// TODO: Handle possible collection/removal
	FMulticastScriptDelegate* Delegate;
	
	static int Construct(lua_State* L, UFunction* SignatureFunction, FMulticastScriptDelegate* Delegate);
	static FLuaFMulticastDelegate* Get(lua_State* L, int Index = 1);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	FString ToString() const;

	static int Lua_Add(lua_State* L);
	static int Lua_Remove(lua_State* L);
	static int Lua_RemoveAll(lua_State* L);
	static int Lua_Clear(lua_State* L);
	static int Lua_Wait(lua_State* L);
	static int Lua_Trigger(lua_State* L);
	static int Lua_Contains(lua_State* L);
	static int Lua_IsBound(lua_State* L);

	static int Lua__index(lua_State* L);
	static int Lua__call(lua_State* L);
	static int Lua__tostring(lua_State* L);
	static int Lua__gc(lua_State* L);

	static void RegisterMetadata(lua_State* L);
	inline static const char* Name = "FMulticastDelegate";

private:
	inline static TArray<luaL_Reg> Metadata = {
		{"__index", Lua__index},
		{"__call", Lua__call},
		{"__tostring", Lua__tostring},
		{"__gc", Lua__gc},
	};

	inline static TMap<FString, lua_CFunction> Methods = {
		{"Add", Lua_Add},
		{"Remove", Lua_Remove},
		{"RemoveAll", Lua_RemoveAll},
		{"Clear", Lua_Clear},
		{"Wait", Lua_Wait},
		{"Trigger", Lua_Trigger},
		{"Contains", Lua_Contains},
		{"IsBound", Lua_IsBound},
	};
};
