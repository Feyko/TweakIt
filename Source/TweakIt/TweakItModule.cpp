#include "TweakItModule.h"

#include "AssetRegistryModule.h"
#include "FactoryGame/Public/Equipment/FGBuildGunDismantle.h"
#include "Logging/FTILog.h"
#include "Patching/NativeHookManager.h"

void FTweakItModule::StartupModule()
{
	LOG("TweakIt 0.6.0-dev starting")
	Orchestrator = new FTIScriptOrchestrator();
	Orchestrator->StartAllScripts();
}

IMPLEMENT_GAME_MODULE(FTweakItModule, TweakIt);
