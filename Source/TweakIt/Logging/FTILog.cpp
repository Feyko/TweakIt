#include "FTILog.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "TweakIt/Lua/Scripting/TIScriptOrchestrator.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

FString FTILog::CurrentScript = "TweakIt";
TMap<FString, FOutputDeviceFile*> FTILog::Files = {};
FOutputDeviceFile* FTILog::TweakItLog = new FOutputDeviceFile(*GetLogFilenameForScript("TweakIt"));

void FTILog::LogForScript(FString String, FString ScriptName, ELogVerbosity::Type Level)
{
	if (ScriptName == "TweakIt" || ScriptName == "")
	{
		TweakItLog->Log(LogTweakIt.GetCategoryName(), Level, String);
		return;
	}
	TweakItLog->Log(LogTweakIt.GetCategoryName(), Level, WrapStringWithScript(String, ScriptName));
	FOutputDeviceFile* LogFile = GetLogFileForScript(ScriptName);
	LogFile->Log(LogTweakIt.GetCategoryName(), Level, String);
}

FOutputDeviceFile* FTILog::GetLogFileForScript(FString ScriptName)
{
	if (FOutputDeviceFile** ExistingFile = Files.Find(ScriptName))
	{
		return *ExistingFile;
	}
	FOutputDeviceFile* NewFile = new FOutputDeviceFile(*GetLogFilenameForScript(ScriptName));
	Files.Add(ScriptName, NewFile);
	return NewFile;
}

FString FTILog::GetLogFilenameForScript(FString ScriptName)
{
	return FPaths::Combine(FTIScriptOrchestrator::GetConfigDirectory(), ScriptName + ".log");
}

FString FTILog::WrapStringWithScript(FString String, FString ScriptName)
{
	if (ScriptName != "" && ScriptName != "TweakIt")
	{
		String = ScriptName + ": " + String;
	}
	return String;
}