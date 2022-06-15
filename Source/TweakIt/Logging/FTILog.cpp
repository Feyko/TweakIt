#include "FTILog.h"

#include "Developer/AITestSuite/Public/TestLogger.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

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
