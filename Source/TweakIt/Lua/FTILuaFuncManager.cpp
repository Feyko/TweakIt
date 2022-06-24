#include "FTILuaFuncManager.h"

#include "TweakIt/Logging/FTILog.h"

TMap<FString, FLuaFunc> FTILuaFuncManager::Funcs = {};

void FLuaFunc::Free()
{
	FMemory::Free(Data);
}

bool FLuaFunc::AddData(const void* NewData, size_t DataSize)
{
	LOG("Adding data")
	bool OK = AddSize(DataSize);
	LOG("Finished adding size")
	if (!OK)
	{
		LOG("Was not ok. Freeing")
		FMemory::Free(Data);
		LOG("Freed")
		Data = nullptr;
		return false;
	}
	LOG("Copying data")
	FMemory::Memcpy(Data + Size - DataSize, NewData, DataSize);
	LOG("Finished")
	return true;
}

const char* FLuaFunc::Read(size_t* OutSize)
{
	size_t BlockSize = GetNextBlockSize();
	if (BlockSize == -1)
	{
		Reset();
		OutSize = 0;
		return nullptr;
	}
	*OutSize = BlockSize;
	uint8* ReturnData = static_cast<uint8*>(FMemory::Malloc(BlockSize));
	if (ReturnData == nullptr)
	{
		MemError = true;
		return nullptr;
	}
	FMemory::Memcpy(ReturnData, Data + DataOffset, BlockSize);
	DataOffset += BlockSize;
	return reinterpret_cast<const char*>(ReturnData);
}

size_t FLuaFunc::GetNextBlockSize()
{
	if (BlockSizes.Num() <= BlockIndex + 1)
	{
		return -1;
	}
	size_t BlockSize = BlockSizes[BlockIndex];
	BlockIndex++;
	return BlockSize;
}

void FLuaFunc::Reset()
{
	DataOffset = 0;
	BlockIndex = 0;
	MemError = false;
}

bool FLuaFunc::AddSize(size_t AddedSize)
{
	LOG("Adding size")
	void* New;
	if (Data == nullptr)
	{
		LOG("IS NULL")
		New = FMemory::Malloc(Size);
	} else
	{
		New = FMemory::Realloc(Data, Size);
	}
	LOGF("%llu", AddedSize)
	if (New == nullptr)
	{
		LOG("INVALID ALLOCATION")
		return false;
	}
	Size += AddedSize;
	BlockSizes.Add(AddedSize);
	Data = static_cast<uint8*>(New);
	return true;
}

FLuaFunc FTILuaFuncManager::DumpFunction(lua_State* L, FString Name, int Index, bool Strip)
{
	if (!lua_isfunction(L, Index) || lua_iscfunction(L, Index))
	{
		luaL_argexpected(L, false, Index, "Lua Function");
	}
	lua_settop(L, lua_gettop(L) + 1);
	lua_copy(L, Index, -1);
	FLuaFunc Desc;
	LOG("Starting dump")
	lua_dump(L, WriterFunc, &Desc, Strip);
	lua_settop(L, -2);
	Funcs.Add(Name, Desc);
	return Desc;
}

int FTILuaFuncManager::LoadFunction(lua_State* L, FLuaFunc Func, FString Name)
{
	int r = lua_load(L, ReaderFunc, &Func, TCHAR_TO_UTF8(*Name), nullptr);
	LOG(r)
	return 1;
}

int FTILuaFuncManager::WriterFunc(lua_State* L, const void* NewData, size_t DataSize, void* Descriptor)
{
	LOG("Writing")
	FLuaFunc* Desc = static_cast<FLuaFunc*>(Descriptor);
	return !Desc->AddData(NewData, DataSize);
}

const char* FTILuaFuncManager::ReaderFunc(lua_State* L, void* Descriptor, size_t* OutSize)
{
	FLuaFunc* Desc = static_cast<FLuaFunc*>(Descriptor);
	const char* Data = Desc->Read(OutSize);
	if (Desc->MemError)
	{
		luaL_error(L, "Failed to allocate memory in Lua Function Reader");
	} 
	return Data;
}
