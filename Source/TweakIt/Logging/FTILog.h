#pragma once
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);

#define LOG(x, ...) LOGL(x, Log, __VA_ARGS__);
#define LOGL(x, level, ...) UE_LOG(LogTweakIt, level, TEXT("%s"), *FTILog::ToFString(x));

#define LOGF(x, ...) LOGFL(x, Log, __VA_ARGS__);
#define LOGFL(x, level, ...) LOG(FString::Printf(TEXT(x), __VA_ARGS__), level);

struct FTILog
{
	static FString ToFString(const char* String);
	static FString ToFString(FString String);
	static FString ToFString(const TCHAR* String);
	static FString ToFString(std::string String);
};
