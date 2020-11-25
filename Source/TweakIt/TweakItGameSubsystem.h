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
	static bool RegisterCommands(UObject* WorldContext);
	static UTweakItGameSubsystem* Get(UObject* WorldContext);

private:
	lua_State* LuaState;
};
