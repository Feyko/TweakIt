#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);

#define LOGL(x, level) UE_LOG(LogTweakIt, level, TEXT(x));
#define LOG(x) LOGL(x, Log);
#define LOGS(x) UE_LOG(LogTweakIt, Log, TEXT("%s"), *FString(x.c_str()));
#define LOGC(x) UE_LOG(LogTweakIt, Log, TEXT("%hs"), x);
#define LOGSL(x, level) UE_LOG(LogTweakIt, level, TEXT("%s"), *FString(x.c_str()));
#define LOGFS(x) LOGFSL(x, Log);
#define LOGFSL(x, level) UE_LOG(LogTweakIt, level, TEXT("%s"), *x);
#define LOGF(x, ...) LOGFS(FString::Printf(TEXT(x), __VA_ARGS__));
#define LOGFL(x, level, ...) LOGFSL(FString::Printf(TEXT(x), __VA_ARGS__), level);

class FTweakItModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }
};
