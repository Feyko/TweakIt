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
	if (ScriptName == "TweakIt")
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

FString FTILog::ToFString(int Int)
{
	return FString::FromInt(Int);
}

FString FTILog::ToFString(int64 Int)
{
	return FString::Printf(TEXT("%lld"), Int);
}

FString FTILog::ToFString(size_t Size)
{
	return FString::Printf(TEXT("%llu"), Size);
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

FString FTILog::ToFString(FName Name)
{
	return Name.ToString();
}

FString FTILog::ToFString(EFunctionFlags Flags)
{
	FString Out;
	const TCHAR* FunctionFlags[32] =
	{
		TEXT("Final"),
		TEXT("0x00000002"),
		TEXT("BlueprintAuthorityOnly"),
		TEXT("BlueprintCosmetic"),
		TEXT("CheatBoard"),
		TEXT("0x00000020"),
		TEXT("Net"),
		TEXT("NetReliable"),
		TEXT("NetRequest"),
		TEXT("Exec"),
		TEXT("Native"),
		TEXT("Event"),
		TEXT("NetResponse"),
		TEXT("Static"),
		TEXT("NetMulticast"),
		TEXT("UbergraphFunction"),
		TEXT("MulticastDelegate"),
		TEXT("Public"),
		TEXT("Private"),
		TEXT("Protected"),
		TEXT("Delegate"),
		TEXT("NetServer"),
		TEXT("HasOutParms"),
		TEXT("HasDefaults"),
		TEXT("NetClient"),
		TEXT("DLLImport"),
		TEXT("BlueprintCallable"),
		TEXT("BlueprintEvent"),		
		TEXT("BlueprintPure"),			
		TEXT("EditorOnly"),
		TEXT("Const"),
		TEXT("NetValidate"),
	};
	
	for (int32 i = 0; i < 32; ++i)
	{
		const uint32 Mask = 1U << i;
		if ((Flags & Mask) != 0)
		{
			Out += FunctionFlags[i];
			Out += "|";
		}
	}
	Out.RemoveFromEnd("|");
	return Out;
}

FString FTILog::ToFString(EPropertyFlags Flags)
{
	FString Out;
	TArray<const TCHAR*> FlagStrings = ParsePropertyFlags(Flags);
	for (auto String : FlagStrings)
	{
		Out += String;
		Out += "|";
	}
	Out.RemoveFromEnd("|");
	return Out;
}

FString FTILog::ToFString(EObjectFlags Flags)
{
	FString Out;
	const TCHAR* ObjectFlags[32] =
	{
		TEXT("Final"),
		TEXT("0x00000002"),
		TEXT("BlueprintAuthorityOnly"),
		TEXT("BlueprintCosmetic"),
		TEXT("CheatBoard"),
		TEXT("0x00000020"),
		TEXT("Net"),
		TEXT("NetReliable"),
		TEXT("NetRequest"),
		TEXT("Exec"),
		TEXT("Native"),
		TEXT("Event"),
		TEXT("NetResponse"),
		TEXT("Static"),
		TEXT("NetMulticast"),
		TEXT("UbergraphFunction"),
		TEXT("MulticastDelegate"),
		TEXT("Public"),
		TEXT("Private"),
		TEXT("Protected"),
		TEXT("Delegate"),
		TEXT("NetServer"),
		TEXT("HasOutParms"),
		TEXT("HasDefaults"),
		TEXT("NetClient"),
		TEXT("DLLImport"),
		TEXT("BlueprintCallable"),
		TEXT("BlueprintEvent"),		
		TEXT("BlueprintPure"),			
		TEXT("EditorOnly"),
		TEXT("Const"),
		TEXT("NetValidate"),
	};
	
	for (int32 i = 0; i < 32; ++i)
	{
		const uint32 Mask = 1U << i;
		if ((Flags & Mask) != 0)
		{
			Out += ObjectFlags[i];
			Out += "|";
		}
	}
	Out.RemoveFromEnd("|");
	return Out;
}
