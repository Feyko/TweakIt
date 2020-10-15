#pragma once
#include "TweakerSubsystem.h"
#include "mod/ModSubsystems.h"

#include "TweakerSubsystemHolder.generated.h"

UCLASS()
class UTWTweakerSubsystemHolder : public UModSubsystemHolder
{
    GENERATED_BODY()
public:
    // Begin UModSubsystemHolder
    virtual void InitSubsystems() override;
    virtual void BeginPlay() override;
    // End UModSubsystemHolder
    UPROPERTY()
    ATWTweakerSubsystem* TweakerSubsystem = nullptr;
};