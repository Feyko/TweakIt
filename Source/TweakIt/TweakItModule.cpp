#include "TweakItModule.h"

#include "AssetRegistryModule.h"
#include "FactoryGame/Public/Equipment/FGBuildGunDismantle.h"
#include "Patching/NativeHookManager.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

void FTweakItModule::StartupModule() {
}

IMPLEMENT_GAME_MODULE(FTweakItModule, TweakIt);

#pragma optimize( "", on )
