#pragma once
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);   

#define LOG(str) LOGL(str, Log);
#define LOGL(str, level) RedirectLogForScript(UE_LOG(LogTweakIt, level, TEXT("%s"), *FTILog::WrapStringWithScript(FTILog::ToFString(str), FTILog::CurrentScript)));

#define LOGF(str, ...) LOGFL(str, Log, __VA_ARGS__);
#define LOGFL(str, level, ...) LOGL(FString::Printf(TEXT(str), __VA_ARGS__), level);

#define RedirectLogForScript(code) FTILog::AddScriptToLog(FTILog::CurrentScript); code; FTILog::RemoveScriptFromLog(FTILog::CurrentScript);

struct FTILog
{
	static void AddScriptToLog(FString ScriptName);
	static void RemoveScriptFromLog(FString ScriptName);

	static FOutputDeviceFile* GetLogFileForScript(FString ScriptName);

	static FString WrapStringWithScript(FString String, FString ScriptName); 

	static thread_local FString CurrentScript;
	
	static FString ToFString(const char* String);
	static FString ToFString(FString String);
	static FString ToFString(const TCHAR* String);
	static FString ToFString(std::string String);
private:
	static TMap<FString, FOutputDeviceFile*> Files;
};
