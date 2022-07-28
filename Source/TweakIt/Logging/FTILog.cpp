#include "FTILog.h"

#include "FGAnimNotify_AkEventCurrentPotential.h"
#include "Developer/AITestSuite/Public/TestLogger.h"
#include "TweakIt/Lua/Scripting/TIScriptOrchestrator.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

FString FTILog::CurrentScript = "TweakIt";
TMap<FString, FOutputDeviceFile*> FTILog::Files = {};
FOutputDeviceFile* FTILog::TweakItLog = new FOutputDeviceFile(*(FTIScriptOrchestrator::GetConfigDirectory() + "TweakIt.log"));

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
	FOutputDeviceFile** ExistingFile = Files.Find(ScriptName);
	if (ExistingFile)
	{
		return *ExistingFile;
	}
	FOutputDeviceFile* NewFile = new FOutputDeviceFile(*(FTIScriptOrchestrator::GetConfigDirectory() + ScriptName + ".log"));
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