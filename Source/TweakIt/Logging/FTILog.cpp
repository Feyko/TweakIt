#include "FTILog.h"

#include "Developer/AITestSuite/Public/TestLogger.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

template <typename... Types>
void FTILog::Log(const char* String, ELogVerbosity::Type Verbosity, Types... Args)
{ 
	FString ToPrint = FString::PrintfImpl(UTF8_TO_TCHAR(String), Args...);
	UELOG(Verbosity,TEXT("%s"), ToPrint);
}

template <typename FmtType, typename... Types>
void FTILog::UELOG(ELogVerbosity::Type Verbosity, FmtType& String, Types... Args)
{
	switch (Verbosity)
	{
	case ELogVerbosity::All: UE_LOG(LogTweakIt, All, String, Args...) break;
	case ELogVerbosity::Fatal: UE_LOG(LogTweakIt, Fatal, String, Args...) break;
	case ELogVerbosity::Error: UE_LOG(LogTweakIt, Error, String, Args...) break;
	case ELogVerbosity::Warning: UE_LOG(LogTweakIt, Warning, String, Args...) break;
	case ELogVerbosity::Display: UE_LOG(LogTweakIt, Display, String, Args...) break;
	case ELogVerbosity::Log: UE_LOG(LogTweakIt, Log, String, Args...) break;
	case ELogVerbosity::Verbose: UE_LOG(LogTweakIt, Verbose, String, Args...) break;
	case ELogVerbosity::SetColor: UE_LOG(LogTweakIt, SetColor, String, Args...) break;
	default: ;
	}
}
