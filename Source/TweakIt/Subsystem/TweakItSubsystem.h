#pragma once

#include "FGSubsystem.h"
#include "Subsystem/ModSubsystem.h"
#include "TweakIt/LuaLib/lua.hpp"
#include "TweakItSubsystem.generated.h"

UCLASS()
class ATweakItSubsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;

	void InitialiseLuaState();
	static void RegisterLuaUClassMetadata(lua_State* L);
	static void RegisterLuaUObjectMetadata(lua_State* L);
	static void RegisterGlobalFunctions(lua_State* L);
	static void RegisterLuaTArrayMetadata(lua_State* L);
	static void RegisterLuaUStructMetadata(lua_State* L);

	UFUNCTION(BlueprintCallable)
	void RunAllScripts();

	UFUNCTION(BlueprintCallable)
	bool RunScript(FString name);

	UFUNCTION(BlueprintCallable)
	bool RegisterCommands();

	static ATweakItSubsystem* Get(UObject* WorldContext);

private:
	lua_State* LuaState = nullptr;
};
