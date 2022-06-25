#include "TweakItSubsystem.h"

#include <Subsystem/SubsystemActorManager.h>

#include "FGGameInstance.h"
#include "TweakIt/Lua/Lua.h"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/Scripting/Script.h"

void ATweakItSubsystem::BeginPlay()
{
	LOG("TweakIt Version 0.6.0-dev is now loaded")
	CreateDefaultScript();
	RunAllScripts();
}


ATweakItSubsystem* ATweakItSubsystem::Get(UObject* WorldContext)
{
	if (!WorldContext->IsValidLowLevel()) { return nullptr; }
	return WorldContext->GetWorld()->GetSubsystem<USubsystemActorManager>()->GetSubsystemActor<ATweakItSubsystem>();
}

bool ATweakItSubsystem::RunAllScripts()
{
	LOG("Running all scripts")
	bool Errored = false;
	StartAllScripts();
	for (auto Script : RunningScripts)
	{
		FScriptState State = Script->WaitForStop();
		if (State == FScriptState::Errored)
		{
			Errored = true;
		}
		if (State.IsCompleted())
		{
			RunningScripts.Remove(Script);
			delete Script;
		}
	}
	return !Errored;
}

void ATweakItSubsystem::StartAllScripts()
{
	LOG("Starting all scripts")
	TArray<FString> Scripts = GetAllScripts();
	for (FString& Filename : Scripts)
	{
		StartScript(Filename);
	}
}

void ATweakItSubsystem::CreateDefaultScript()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();

	if (Manager.DirectoryExists(*ConfigDirectory))
	{
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

bool ATweakItSubsystem::StartScript(FString Name)
{
	LOGF("Running script \"%s\"", *Name)
	FString Path = GetConfigDirectory() + Name;
	if (!FPaths::FileExists(Path))
	{
		return false;
	}
	FScript* Script = new FScript(Path);
	Script->Start();
	RunningScripts.Add(Script);
	return true;
}

TArray<FString> ATweakItSubsystem::GetAllScripts()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	TArray<FString> Filenames;
	Manager.FindFiles(Filenames, *ConfigDirectory, TEXT(".lua"));
	return Filenames;
}
