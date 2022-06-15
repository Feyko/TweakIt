#pragma once
#include "FGCustomizationRecipe.h"
#include "FactoryGame/Public/Equipment/FGBuildGun.h"
#include "FGRecipe.h"
#include "ItemAmount.h"
#include "Hologram/FGHologram.h"

#include "TweakItTesting.generated.h"

enum class EHologramMaterialState : uint8;
UCLASS(Blueprintable)
class UTweakItTesting : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool Boolean;

	UPROPERTY(EditAnywhere)
	int number = 10;

	UPROPERTY(EditAnywhere)
	uint16 ayo = 40;

	UPROPERTY(EditAnywhere)
	float bro = 1.22;

	UPROPERTY(EditAnywhere)
	FString String = "henlo";

	UPROPERTY(EditAnywhere)
	FName Name = "myname";

	UPROPERTY(EditAnywhere)
	FText Text = FText::FromString("sometext");

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFGRecipe> Recipe = UFGCustomizationRecipe::StaticClass();

	UPROPERTY(EditAnywhere)
	EHologramMaterialState Enum = EHologramMaterialState::HMS_OK;

	UPROPERTY(EditAnywhere)
	FItemAmount Item = FItemAmount(UFGItemDescriptor::StaticClass(), 69);

	UPROPERTY(EditAnywhere)
	TArray<FItemAmount> Items = {
		FItemAmount(UFGItemDescriptor::StaticClass(), 69), FItemAmount(UFGItemDescriptor::StaticClass(), 47)
	};

	UPROPERTY(EditAnywhere)
	TArray<int> Numbers = {54, 12, 154};

	UPROPERTY()
	TArray<EBuildGunState> states = {EBuildGunState::BGS_MAX, EBuildGunState::BGS_MENU};

	UFUNCTION(BlueprintCallable)
	static void Testing();
};
