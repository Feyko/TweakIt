#include "TIContentRegistration.h"


#include "FGRecipe.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Logging/FTILog.h"

void FTIContentRegistration::UnlockRecipe(UClass* Recipe, UObject* WorldContext) {
	LOG("Unlocking a recipe")
	if(!Recipe->IsValidLowLevel()) {
		LOG("Recipe isn't valid")
		return;
	}
	if(!Recipe->IsChildOf(UFGRecipe::StaticClass())) {
		LOG("Recipe isn't a recipe")
		return;
	}
	if(!WorldContext->IsA(AActor::StaticClass())) {
		LOG("WorldContext isn't an Actor")
	}
	if(!WorldContext->IsValidLowLevel()) {
		LOG("WorldContext isn't valid")
        return;
	}
	LOG(WorldContext->GetFullName())
	UWorld* world = WorldContext->GetWorld();
	if(!world->IsValidLowLevel()) {
		LOG("World isn't valid")
		return;
	}
	AFGRecipeManager* Manager = AFGRecipeManager::Get(world);
	if (!Manager->IsValidLowLevel()) {
		LOG("The recipe manager isn't valid")
        return;
	}
	if (Manager->IsRecipeAvailable(Recipe)) {
		LOG("Recipe is already unlocked")
        return;
	}
	Manager->AddAvailableRecipe(Recipe);
}
