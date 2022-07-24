#pragma once
#include "Lua.h"
#include "TweakIt/Helpers/Result.h"

struct FLuaFunc {
	FLuaFunc() = delete;
	explicit FLuaFunc(lua_State* L);
	
	void Free();
	bool AddData(const void* NewData, size_t DataSize);

	const char* GetData();
	size_t Size();

	lua_State* L;
private:
	TArray<uint8>* Buf;
};

class FTILuaFuncManager
{
public:
	static FLuaFunc DumpFunction(lua_State* L, FString Name, int Index = -1, bool Strip = false);
	static int LoadFunction(lua_State* L, FLuaFunc Func, FString Name);
	static int LoadSavedFunction(lua_State* L, FString Name);
	static FLuaFunc* GetSavedLuaFunc(FString Name);
	
	static FNativeFuncPtr SavedLuaFuncToNativeFunc(lua_State* L, FString Name);

private:
	static int WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor);
	static void LuaCallerFunc(UObject* Context, FFrame& TheStack, void* const Z_Param__Result);

	static TMap<FString, FLuaFunc> SavedLuaFuncs;
};


