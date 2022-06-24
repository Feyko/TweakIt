#pragma once
#include "Lua.h"

struct FLuaFunc {
	void Free();
	
	bool AddData(const void* NewData, size_t DataSize);
	const char* Read(size_t* OutSize);

	bool MemError = false;
	size_t Size = 0;
	uint8* Data = nullptr;
private:
	size_t GetNextBlockSize();
	void Reset();

	bool AddSize(size_t Size);
	
	int BlockIndex = 0;
	TArray<size_t> BlockSizes = {};
	size_t DataOffset = 0;
};

class FTILuaFuncManager
{
public:
	static FLuaFunc DumpFunction(lua_State* L, FString Name, int Index = -1, bool Strip = false);
	static int LoadFunction(lua_State* L, FLuaFunc Func, FString Name);

	static TMap<FString, FLuaFunc> Funcs;
private:
	static int WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor);
	static const char* ReaderFunc(lua_State* L, void* Data, size_t* OutSize);
};


