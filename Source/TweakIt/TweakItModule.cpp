#include "TweakItModule.h"


#include "AssetRegistryModule.h"
#include "FGBuildGunDismantle.h"
#include "Lua/Lua.h"
#include "Lua/LuaUClass.h"
#include "SML/mod/hooking.h"
#include "util/Logging.h"




void FTweakItModule::StartupModule() {
}

IMPLEMENT_GAME_MODULE(FTweakItModule, TweakIt);

#pragma optimize( "", on )