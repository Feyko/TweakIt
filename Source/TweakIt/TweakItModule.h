#pragma once

#include "Modules/ModuleManager.h"
#include "Subsystem/TIScriptOrchestrator.h"


class FTweakItModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }

	FTIScriptOrchestrator* Orchestrator;
};
