#pragma once
#include "FTILuaFuncManager.h"
#include "Lua.h"
#include "TweakIt/Helpers/Result.h"

struct FLuaFunc {
	// FLuaFunc() = delete;
	explicit FLuaFunc(lua_State* L);
	
	bool AddData(const void* NewData, size_t DataSize);

	const char* GetData();
	size_t Size();

	lua_State* L;
private:
	TArray<uint8> Buf;
};

class FTILuaFuncManager
{
public:
	static FLuaFunc DumpFunction(lua_State* L, FString Name, int Index = -1, bool Strip = false);
	static int LoadFunction(lua_State* L, FLuaFunc Func, FString Name);
	static int LoadSavedFunction(lua_State* L, FString Name);
	static TResult<FLuaFunc> GetSavedLuaFunc(FString Name);
	
	static FNativeFuncPtr SavedLuaFuncToNativeFunc(lua_State* L, FString Name);

	static TPair<UObject*, FName> MakeGlobalLuaUFunction(lua_State* L, UFunction* Signature, int Index);

private:
	static int WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor);
	static void LuaCallerFunc(UObject* Context, FFrame& TheStack, void* const Z_Param__Result);
public:
	static TMap<FString, FLuaFunc> SavedLuaFuncs;
};


