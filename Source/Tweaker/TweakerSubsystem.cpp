#include "TweakerSubsystem.h"


#include "SML/util/ReflectionHelper.h"
#include "FGBuildablePole.h"
#include "FGWheeledVehicleMovementComponent4W.h"
#include "ObjectEditorUtils.h"
#include "TweakerSubsystemHolder.h"
#include "Lua/Lua.h"
#include "mod/hooking.h"
#include "mod/ModSubsystems.h"
#include "util/Logging.h"
#include "util/Utility.h"

#define OVERRIDE_BODY_MULTIPLIER(Name, arguments) \
float overrideValue = Config->GetNumberField(FString(Name) + " - Value"); \
bool useMultiplier = Config->GetBoolField(FString(Name) + " - Use Multiplier"); \
int multiplier = Config->GetNumberField(FString(Name) + " - Multiplier"); \
if (useMultiplier) \
{ \
    scope.Override(scope(arguments) * multiplier); \
} \
else \
{ \
    scope.Override(overrideValue); \
}

#define OVERRIDE_BODY_DIVIDER(Name, arguments) \
float overrideValue = Config->GetNumberField(FString(Name) + " - Value"); \
bool useMultiplier = Config->GetBoolField(FString(Name) + " - Use Multiplier"); \
int multiplier = Config->GetNumberField(FString(Name) + " - Multiplier"); \
if (useMultiplier) \
{ \
    scope.Override(scope(arguments) / multiplier); \
} \
else \
{ \
    scope.Override(overrideValue); \
}


#define EXPOSE_OPTION(Name, DefaultValue, MethodReference, Handler) \
ATWTweakerSubsystem::ExposeCppOption(Name, DefaultValue); \
SUBSCRIBE_METHOD(MethodReference, Handler);

void ATWTweakerSubsystem::BeginPlay()
{
    SML::Logging::warning("ATWTS beginplay");
    UFETLua::test();
    // TSharedRef<FJsonObject> defaultConfig = MakeShareable(new FJsonObject()) ;
    // TSharedRef<FJsonObject> defaultObject = MakeShareable(new FJsonObject()) ;
    // TArray < TSharedPtr < FJsonValue >> array;
    // defaultObject->SetStringField("Name", "required");
    // defaultObject->SetStringField("Path" ,"required");
    // defaultObject->SetStringField("ValueType" ,"required");
    // defaultObject->SetStringField("Value", "required");
    // defaultObject->SetStringField("ParentPath", "optional");
    // array.Add(MakeShareable(new FJsonValueObject(defaultObject)));
    // defaultConfig->SetArrayField("Overrides",array);
    // Config = SML::ReadModConfig("Tweaker", defaultConfig);
    // TArray<TSharedPtr<FJsonValue>> Overrides = Config->GetArrayField("Overrides");
    // for (auto& Override : Overrides)
    // {
    //     SML::Logging::warning("Checking an object");
    //     TSharedPtr<FJsonObject> OverrideObject = Override->AsObject();
    //     FString Name = OverrideObject->GetStringField("Name");
    //     FString ValueType = OverrideObject->GetStringField("ValueType");
    //     FString Path = OverrideObject->GetStringField("Path");
    //     FString Value = OverrideObject->GetStringField("Value");
    //     FString ParentPath = "";
    //     bool IsChild = OverrideObject->TryGetStringField("ParentPath", ParentPath);
    //     UClass* Class = nullptr;
    //     Class = LoadObject<UClass>(NULL,*Path);
    //     SML::Logging::warning("Class is valid.");
    //     UObject* Object = nullptr;
    //     if (IsChild)
    //     {
    //         UClass* ParentClass = LoadObject<UClass>(NULL, *ParentPath);
    //         AActor* Parent = static_cast<AActor*>(ParentClass->GetDefaultObject());
    //         if (!ParentClass->GetDefaultObject()->IsValidLowLevelFast())
    //         {
    //             SML::Logging::warning("ParentClass CDO isn't valid");
    //         }
    //         if (!Parent->IsValidLowLevelFast())
    //         {
    //             SML::Logging::warning("Parent isn't an actor");
    //         }
    //         // Object = FindDefaultComponentByName(ParentClass, Class, "");
    //     }
    //     else
    //     {
    //         Object = Class->GetDefaultObject();
    //     }
    //     if (Object)
    //     {
    //         SML::Logging::warning("The Object is valid");
    //         ExposeUPropertyOptions(Name, ValueType, Object, Value);
    //     }
    // }
    SML::Logging::warning("Finished");
  //   EXPOSE_OPTION("Stack Size", 500, UFGItemDescriptor::GetStackSize, [=](auto& scope, TSubclassOf<UFGItemDescriptor> item)
  // {
  // OVERRIDE_BODY_MULTIPLIER("Stack Size", item)
  // });
    
}

ATWTweakerSubsystem* ATWTweakerSubsystem::GetTweakerSubsystem(UObject* WorldContext)
{
    return GetSubsystemHolder<UTWTweakerSubsystemHolder>(WorldContext)->TweakerSubsystem;
}

void ATWTweakerSubsystem::ExposeUPropertyOptions(FString Name, FString Type, UObject* Object, FString DefaultValue)
{
    SML::Logging::warning(*Name);
    SML::Logging::warning(*Type);
    SML::Logging::warning(*Object->GetName());
    SML::Logging::warning(*DefaultValue);
    if (Type == "Float")
    {
        Internal_ExposeUPropertyOptions<UFloatProperty>(Name, Object, FCString::Atof(*DefaultValue));
    }
    else if (Type == "int32")
    {
        Internal_ExposeUPropertyOptions<UIntProperty>(Name, Object, FCString::Atoi(*DefaultValue));
    }
}

void ATWTweakerSubsystem::ExposeCppOption(FString Name, float DefaultValue)
{
    CppOptions.Add(Name);
    if (!Config->HasField(Name))
    {
        Config->SetBoolField(Name + " - State", false);
        Config->SetNumberField(Name + " - Multiplier", 5);
        Config->SetBoolField(Name + " - Use Multiplier", false);
        Config->SetNumberField(Name + " - Value", DefaultValue);
        Config->SetNumberField(Name + " - Default Value", DefaultValue);
        SML::WriteModConfig("Tweaker", Config);
    }
}

// TMap<UClass,TArray<FString>> ATWTweakerSubsystem::UPropertyOptions = {};

template <typename T>
void ATWTweakerSubsystem::Internal_ExposeUPropertyOptions(FString Name, UObject* Object, const typename T::TCppType& DefaultValue)
{
    SML::Logging::warning(*FString::Printf(TEXT("Exposing %s as a UProperty option for %s"), *Name, *Object->GetName()));
    T* Property = FReflectionHelper::FindPropertyByShortName<T>(Object->GetClass(), *Name);
    UProperty* CastedProperty = Cast<UProperty>(Property);
    if(Property)
    {
        SML::Logging::warning("Property was found");
    }
    
    if(!CastedProperty)
    {
        SML::Logging::warning("Casting failed");
        return;
    }
    if (!Config->HasField(Name + " - Value"))
    {
        SML::Logging::warning("Creating the config values for the property");
        Config->SetBoolField(Name + " - State", true);
        Config->SetNumberField(Name + " - Value", DefaultValue);
        Config->SetNumberField(Name + " - Default Value", DefaultValue);
        SML::WriteModConfig("Tweaker", Config);
    }
    if (Config->GetBoolField(Name + " - State"))
    {
        SML::Logging::warning("Overwriting the property value");
        FReflectionHelper::SetPropertyValue<T>(Object, *Name, Config->GetNumberField(Name + " - Value"));
    }
}





void ATWTweakerSubsystem::Testing()
{
    
}

TArray<FString> ATWTweakerSubsystem::CppOptions = {};

TSharedRef<FJsonObject> ATWTweakerSubsystem::Config = MakeShareable(new FJsonObject());
