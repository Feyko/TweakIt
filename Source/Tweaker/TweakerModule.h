#pragma once

#include "Modules/ModuleManager.h"

class FTweakerModule : public FDefaultGameModuleImpl {
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }

	virtual void PostLoadCallback() override;
};