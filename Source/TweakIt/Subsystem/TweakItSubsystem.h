#pragma once

#include "FGSubsystem.h"
#include "Subsystem/ModSubsystem.h"
#include "TweakIt/Lua/LuaState.h"
#include "TweakItSubsystem.generated.h"

UCLASS()
class ATweakItSubsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool RunAllScripts();

	UFUNCTION(BlueprintCallable)
	bool RunScript(FString Name);

	TArray<FString> GetAllScripts();
	
	void CreateDefaultScript();
	FString GetConfigDirectory();
	
	static ATweakItSubsystem* Get(UObject* WorldContext);

private:
	FLuaState LuaState = FLuaState(this);
};
