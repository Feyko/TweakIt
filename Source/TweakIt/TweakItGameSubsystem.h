#pragma once
#include "CoreGlobals.h"
#include "GameInstanceSubsystem.h"
#include "JsonObject.h"
#include "LuaLib/lua.hpp"
#include "TweakItGameSubsystem.generated.h"
UCLASS()
class UTweakItGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void InitialiseLuaState();
	void RunAllScripts();
	bool RunScript(FString name);
	UFUNCTION(BlueprintCallable)
	bool RegisterCommands(UObject* WorldContext);
	static UTweakItGameSubsystem* Get(UObject* WorldContext);
private:
	lua_State* LuaState;
};
