#pragma once
#include "EngineGlobals.h"
#include "FactoryGame/Public/Equipment/FGBuildGun.h"
#include "FGRecipe.h"
#include "ItemAmount.h"

#include "TweakItTesting.generated.h"

UCLASS(Blueprintable)
class UTweakItTesting : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int number = 10;
	
	UPROPERTY(EditAnywhere)
	FItemAmount Item = FItemAmount(UFGItemDescriptor::StaticClass(), 69);

	UPROPERTY(EditAnywhere)
	TArray<FItemAmount> Items = {FItemAmount(UFGItemDescriptor::StaticClass(), 69), FItemAmount(UFGItemDescriptor::StaticClass(), 47)};

	UPROPERTY(EditAnywhere)
	TArray<int> Numbers = {54,12,154};

	UPROPERTY()
	TArray<EBuildGunState> states = {EBuildGunState::BGS_MAX, EBuildGunState::BGS_MENU};
	
	UFUNCTION(BlueprintCallable)
	static void Testing();
};
