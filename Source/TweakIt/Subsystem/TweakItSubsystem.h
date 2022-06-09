#pragma once

#include "FGSubsystem.h"
#include "Subsystem/ModSubsystem.h"
#include "TweakIt/Lua/LuaState.h"
#include "TweakIt/Lua/Script.h"
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
	void StartAllScripts();
	
	UFUNCTION(BlueprintCallable)
	bool StartScript(FString Name);

	static TArray<FString> GetAllScripts();

	static void CreateDefaultScript();
	static FString GetConfigDirectory();
	
	static ATweakItSubsystem* Get(UObject* WorldContext);

	TArray<FScript*> RunningScripts;

private:
	FLuaState LuaState = FLuaState(this);
};
