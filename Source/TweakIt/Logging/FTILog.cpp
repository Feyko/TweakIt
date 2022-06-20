#include "FTILog.h"

#include "Developer/AITestSuite/Public/TestLogger.h"
#include "TweakIt/Subsystem/TweakItSubsystem.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

thread_local FString FTILog::CurrentScript = "";
TMap<FString, FOutputDeviceFile*> FTILog::Files = {};

void FTILog::AddScriptToLog(FString ScriptName)
{
	if (ScriptName == "")
	{
		return;
	}
	FOutputDeviceFile* LogFile = GetLogFileForScript(ScriptName);
	GLog->AddOutputDevice(LogFile);
}

void FTILog::RemoveScriptFromLog(FString ScriptName)
{
	if (ScriptName == "")
	{
		return;
	}
	FOutputDeviceFile* LogFile = GetLogFileForScript(ScriptName);
	GLog->RemoveOutputDevice(LogFile);
}

FOutputDeviceFile* FTILog::GetLogFileForScript(FString ScriptName)
{
	FOutputDeviceFile** ExistingFile = Files.Find(ScriptName);
	if (ExistingFile)
	{
		return *ExistingFile;
	}
	FOutputDeviceFile* NewFile = new FOutputDeviceFile(*(ATweakItSubsystem::GetConfigDirectory() + ScriptName + ".log"));
	Files.Add(ScriptName, NewFile);
	return NewFile;
}

FString FTILog::WrapStringWithScript(FString String, FString ScriptName)
{
	LOGF("Wrapping %ls", *String)
	LOG(ScriptName)
	if (ScriptName != "" && ScriptName != "TweakIt")
	{
		String = ScriptName + ": " + String;
	}
	return String;
}

FString FTILog::ToFString(const char* String)
{
	return FString(String);
}

FString FTILog::ToFString(FString String)
{
	return String;
}

FString FTILog::ToFString(const TCHAR* String)
{
	return FString(String);
}

FString FTILog::ToFString(std::string String)
{
	return FString(String.c_str());
}