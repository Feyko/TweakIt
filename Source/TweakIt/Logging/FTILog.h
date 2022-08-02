#pragma once

#include "TweakIt/Helpers/StringConv.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);   

#define LOG(str) LOGL(str, Log)
#define LOGL(str, level)\
	UE_LOG(LogTweakIt, level, TEXT("%s"), *FTILog::WrapStringWithScript(FStringConv::ToFString(str), FTILog::CurrentScript));\
	FTILog::LogForScript(FStringConv::ToFString(str), FTILog::CurrentScript, ELogVerbosity::level);

#define LOGF(str, ...) LOGFL(str, Log, __VA_ARGS__);
#define LOGFL(str, level, ...) LOGL(FString::Printf(TEXT(str), __VA_ARGS__), level)

struct FTILog
{
	static void LogForScript(FString String, FString ScriptName, ELogVerbosity::Type Level);

	static FOutputDeviceFile* GetLogFileForScript(FString ScriptName);
	static FString GetLogFilenameForScript(FString ScriptName);

	static FString WrapStringWithScript(FString String, FString ScriptName);
	
	static FString CurrentScript;
private:
	static TMap<FString, FOutputDeviceFile*> Files;
	static FOutputDeviceFile* TweakItLog;
};
