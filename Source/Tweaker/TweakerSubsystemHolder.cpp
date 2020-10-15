#include "TweakerSubsystemHolder.h"

#include "TweakerSubsystem.h"
#include "SML/util/Logging.h"
void UTWTweakerSubsystemHolder::InitSubsystems()
{
    SML::Logging::warning("SPAWNING CM SUBSYSTEM");
    SpawnSubsystem(TweakerSubsystem, ATWTweakerSubsystem::StaticClass(), "TweakerSubsystem");
}

void UTWTweakerSubsystemHolder::BeginPlay()
{
    SML::Logging::warning("CM SUB HOLDER HAS SPAWNED");
    SML::Logging::warning(IsValid(TweakerSubsystem));
    SML::Logging::warning(IsValid(ATWTweakerSubsystem::GetTweakerSubsystem(this)));   
}