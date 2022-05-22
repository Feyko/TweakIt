#include "TweakItModule.h"

#include "AssetRegistryModule.h"
#include "SatisfactoryModLoader.h"
#include "FactoryGame/Public/Equipment/FGBuildGunDismantle.h"
#include "Patching/NativeHookManager.h"

DEFINE_LOG_CATEGORY(LogTweakIt)

void FTweakItModule::StartupModule() {
	// SUBSCRIBE_METHOD(AActor::NotifyActorOnReleased, [](auto& scope, AActor* self, FKey) {
	// 	LOGF("Notifying Actor on release for %s", *self->GetFullName())
	// })
}

IMPLEMENT_GAME_MODULE(FTweakItModule, TweakIt);

#pragma optimize( "", on )
