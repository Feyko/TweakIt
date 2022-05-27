#pragma once

#include "FGSubsystem.h"
#include "Subsystem/ModSubsystem.h"
#include "TweakIt/Lua\lib/lua.hpp"
#include "TweakItSubsystem.generated.h"

UCLASS()
class ATweakItSubsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;

	void InitialiseLuaState();
	static void RegisterMetadatas(lua_State* L);
	static void RegisterGlobalFunctions(lua_State* L);

	UFUNCTION(BlueprintCallable)
	bool RunAllScripts();

	UFUNCTION(BlueprintCallable)
	bool RunScript(FString name);

	static ATweakItSubsystem* Get(UObject* WorldContext);

private:
	lua_State* LuaState = nullptr;
};
