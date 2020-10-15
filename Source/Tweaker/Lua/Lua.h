#pragma once


#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "FGBuildable.h"
#include "LuaLib/lua.hpp"
#include "util/Logging.h"
#include "Lua.generated.h"


#define LOG(x) UE_LOG(LogTemp, Log, TEXT(x)); SML::Logging::warning(TEXT(x));
#define LOGC(x, level) UE_LOG(LogTemp, level, TEXT(x)); SML::Logging::warning(TEXT(x));
#define LOGS(x) UE_LOG(LogTemp, Log, TEXT("%s"), *FString(x.c_str())); SML::Logging::warning(*FString(x.c_str()));
#define LOGFS(x) UE_LOG(LogTemp, Log, TEXT("%s"), *x); SML::Logging::warning(*x);

UCLASS(Blueprintable)
class UFETLua : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static void test();
    
    UPROPERTY(EditDefaultsOnly)
    TArray<FString> Strings = {"Ouais ouais ouais", "WOAW", "trop lol"};

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UFETLua> AClass;
    
    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UClass>> Classes;

};

namespace Tweaker
{
    namespace Lua
    {
        bool CheckLua(lua_State* L, int r);

        void stackDump (lua_State *L);

    	// Mostly borrowed from FIN's source. Thanks Pana !
        void propertyToLua(lua_State* L, UProperty* p, void* data);

    	// Mostly borrowed from FIN's source. Thanks Pana !
		void luaToProperty(lua_State* L, UProperty* p, void* data, int i);
        
        int lua_GetClass(lua_State* L);

        int lua_Print(lua_State* L);
    }
}