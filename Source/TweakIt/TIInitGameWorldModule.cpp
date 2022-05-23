// Fill out your copyright notice in the Description page of Project Settings.

#include "TIInitGameWorldModule.h"


#include <cassert>
#include <Subsystem/SubsystemActorManager.h>


#include "Commands/TIRunAllScriptsCommand.h"
#include "Commands/TIRunScriptCommand.h"
#include "Subsystem/TweakItSubsystem.h"

UTIInitGameWorldModule::UTIInitGameWorldModule() {
#if !WITH_EDITOR
	UE_LOG(LogActor, Log, TEXT("TWEAKIT WORLD MODULE SPAWNING"))
	bRootModule = true;
	ModSubsystems.Add(ATweakItSubsystem::StaticClass());
	mChatCommands = {ATIRunScriptCommand::StaticClass(), ATIRunAllScriptsCommand::StaticClass()};
#endif
}
