#pragma once
#include "Lua.h"

struct FLuaFunc {
	FLuaFunc();
	
	void Free();
	bool AddData(const void* NewData, size_t DataSize);

	const char* GetData();
	size_t Size();
private:
	TArray<uint8>* Buf;
};

class FTILuaFuncManager
{
public:
	static FLuaFunc DumpFunction(lua_State* L, FString Name, int Index = -1, bool Strip = false);
	static int LoadFunction(lua_State* L, FLuaFunc Func, FString Name);

	static TMap<FString, FLuaFunc> Funcs;
private:
	static int WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor);
};


