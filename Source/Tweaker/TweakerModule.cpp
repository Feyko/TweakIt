#include "TweakerModule.h"


#include "AssetRegistryModule.h"
#include "FGBuildGunDismantle.h"
#include "TweakerSubsystemHolder.h"
#include "Lua/Lua.h"
#include "Lua/LuaUClass.h"
#include "SML/mod/hooking.h"
#include "util/Logging.h"




void FTweakerModule::StartupModule() {
    FSubsystemInfoHolder::RegisterSubsystemHolder(UTWTweakerSubsystemHolder::StaticClass());
    UFETLua::test();
}

void FTweakerModule::PostLoadCallback()
{
    SML::Logging::warning("Allez quoi");
}

IMPLEMENT_GAME_MODULE(FTweakerModule, Tweaker);

#pragma optimize( "", on )