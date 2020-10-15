#pragma once

#include "FGBuildable.h"
#include "FGSubsystem.h"
#include "util/Logging.h"

#include "TweakerSubsystem.generated.h"    

UCLASS()
class ATWTweakerSubsystem : public AFGSubsystem
{
    GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    static void Testing();
    
    static ATWTweakerSubsystem* GetTweakerSubsystem(UObject* WorldContext);

    static void ExposeUPropertyOptions(FString Name, FString Type, UObject* Object, FString DefaultValue);
    
    template <typename T>
    static void Internal_ExposeUPropertyOptions(FString Name, UObject* Object, const typename T::TCppType&  DefaultValue);
    
    static UActorComponent* FindDefaultComponentByName(TSubclassOf<AActor> InActorClass, TSubclassOf<UActorComponent> InComponentClass, FString ComponentName);
private:
    static void ExposeCppOption(FString name, float defaultValue);
    
    static TSharedRef<FJsonObject> Config;
    static TArray<FString> CppOptions;
    // static TMap<UClass,TArray<FString>> UPropertyOptions;
};


