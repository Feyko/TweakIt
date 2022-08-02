#pragma once
#include <string>

#include "Result.h"

struct FStringConv
{
	template<typename... T>
	static FString Printf(FString String, T... Args);
	
	static FString ToFString(const char* String);
	static FString ToFString(FString String);
	static FString ToFString(const TCHAR* String);
	static FString ToFString(std::string String);
	static FString ToFString(FName Name);
	static FString ToFString(EFunctionFlags Flags);
	static FString ToFString(EPropertyFlags Flags);
	static FString ToFString(EObjectFlags Flags);
	static FString ToFString(int Int);
	static FString ToFString(bool Bool);
	static FString ToFString(int64 Int);
	static FString ToFString(size_t Size);
	template<typename V, typename E>
	static FString ToFString(TResult<V, E> Result);
};
