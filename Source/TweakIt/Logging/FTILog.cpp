#include "FTILog.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "Developer/AITestSuite/Public/TestLogger.h"
#include "TweakIt/Subsystem/TweakItSubsystem.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

thread_local FString FTILog::CurrentScript = "TweakIt";
TMap<FString, FOutputDeviceFile*> FTILog::Files = {};
FOutputDeviceFile* FTILog::TweakItLog = new FOutputDeviceFile(*(ATweakItSubsystem::GetConfigDirectory() + "TweakIt.log"));

void FTILog::LogForScript(FString String, FString ScriptName, ELogVerbosity::Type Level)
{
	if (ScriptName == "TweakIt")
	{
		TweakItLog->Log(LogTweakIt.GetCategoryName(), Level, String);
		return;
	}
	TweakItLog->Log(LogTweakIt.GetCategoryName(), Level, WrapStringWithScript(String, ScriptName));
	FOutputDeviceFile* LogFile = GetLogFileForScript(ScriptName);
	LogFile->Log(LogTweakIt.GetCategoryName(), Level, String);
	TweakItLog->Flush();
	LogFile->Flush();
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
	if (ScriptName != "" && ScriptName != "TweakIt")
	{
		String = ScriptName + ": " + String;
	}
	return String;
}

FString FTILog::ToFString(int Int)
{
	return FString::FromInt(Int);
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