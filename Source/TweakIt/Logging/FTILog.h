#pragma once
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);   

#define LOG(str) LOGL(str, Log)
#define LOGL(str, level)\
	UE_LOG(LogTweakIt, level, TEXT("%s"), *FTILog::WrapStringWithScript(FTILog::ToFString(str), FTILog::CurrentScript));\
	FTILog::LogForScript(FTILog::ToFString(str), FTILog::CurrentScript, ELogVerbosity::level);

#define LOGF(str, ...) LOGFL(str, Log, __VA_ARGS__);
#define LOGFL(str, level, ...) LOGL(FString::Printf(TEXT(str), __VA_ARGS__), level)

struct FTILog
{
	static void LogForScript(FString String, FString ScriptName, ELogVerbosity::Type Level);

	static FOutputDeviceFile* GetLogFileForScript(FString ScriptName);

	static FString WrapStringWithScript(FString String, FString ScriptName);
	
	static thread_local FString CurrentScript;
	
	static FString ToFString(const char* String);
	static FString ToFString(FString String);
	static FString ToFString(const TCHAR* String);
	static FString ToFString(std::string String);
	static FString ToFString(FName Name);
	static FString ToFString(EFunctionFlags Flags);
	static FString ToFString(int Int);
	static FString ToFString(size_t Size);
private:
	static TMap<FString, FOutputDeviceFile*> Files;
	static FOutputDeviceFile* TweakItLog;
};
