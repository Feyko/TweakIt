#include "TweakItSubsystem.h"

#include <Subsystem/SubsystemActorManager.h>

#include "FGGameInstance.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "TweakIt/Lua/Lua.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/TweakItModule.h"

void ATweakItSubsystem::BeginPlay() {
	LOG("TweakIt Version 0.6.0-dev is now loaded")
	CreateDefaultScript();
	RunAllScripts();
}


ATweakItSubsystem* ATweakItSubsystem::Get(UObject* WorldContext) {
	if (!WorldContext->IsValidLowLevel()) { return nullptr; }
	return WorldContext->GetWorld()->GetSubsystem<USubsystemActorManager>()->GetSubsystemActor<ATweakItSubsystem>();
}

bool ATweakItSubsystem::RunAllScripts() {
	LOG("Running all scripts")
	TArray<FString> Scripts = GetAllScripts();
	for (FString& Filename : Scripts) {
		bool OK = RunScript(Filename);
		if (!OK)
		{
			return false;
		}
	}
	return true;
}

void ATweakItSubsystem::CreateDefaultScript()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	
	if (Manager.DirectoryExists(*ConfigDirectory)) {
		return;
	}
	
	LOG("Creating the default directory and script")
	Manager.MakeDirectory(*ConfigDirectory);
	FArchive* file = Manager.CreateFileWriter(*ConfigDirectory.Append("script.lua"));
	file->Close();
}

FString ATweakItSubsystem::GetConfigDirectory()
{
	return UConfigManager::GetConfigurationFolderPath().Append("/TweakIt/");
}

bool ATweakItSubsystem::RunScript(FString Name) {
	LOGF("Running script \"%s\"", *Name)
	FString Path = GetConfigDirectory() + Name;
	if (!FPaths::FileExists(Path)) { return false; }
	return CheckLua(LuaState.L, luaL_dofile(LuaState.L, TCHAR_TO_UTF8(*Path)));
}

TArray<FString> ATweakItSubsystem::GetAllScripts()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	TArray<FString> Filenames;
	Manager.FindFiles(Filenames, *ConfigDirectory, TEXT(".lua"));
	return Filenames;
}
