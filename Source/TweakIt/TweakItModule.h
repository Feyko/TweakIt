#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTweakIt, Log, Log);

#define LOG(x) UE_LOG(LogTweakIt, Log, TEXT(x));
#define LOGC(x, level) UE_LOG(LogTweakIt, level, TEXT(x));
#define LOGS(x) UE_LOG(LogTweakIt, Log, TEXT("%s"), *FString(x.c_str()));
#define LOGFS(x) UE_LOG(LogTweakIt, Log, TEXT("%s"), *x);
#define LOGF(x, ...) LOGFS(FString::Printf(TEXT(x), __VA_ARGS__))

class FTweakItModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }
};
