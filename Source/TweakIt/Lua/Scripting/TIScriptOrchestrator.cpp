#include "TIScriptOrchestrator.h"

#include "FGGameInstance.h"
#include "TweakIt/Lua/Lua.h"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "ModLoading/ModLoadingLibrary.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/Scripting/Script.h"

FTIScriptOrchestrator::FTIScriptOrchestrator()
{
	UModLoadingLibrary* ModLoadingLibrary = GEngine->GetEngineSubsystem<UModLoadingLibrary>();
	for (auto Mod : ModLoadingLibrary->GetLoadedMods())
	{
		LOG(Mod.Name)
	}
	CreateDefaultScript();
}

FTIScriptOrchestrator::~FTIScriptOrchestrator()
{
	for (auto Script : RunningScripts)
	{
		delete Script;
	}
}

bool FTIScriptOrchestrator::StartAllScripts()
{
	LOG("Running all scripts")
	bool Errored = false;
	TArray<FString> Scripts = GetAllScripts();
	for (FString& Filename : Scripts)
	{
		FScriptState State = StartScript(Filename);
	}
	return !Errored;
}

void FTIScriptOrchestrator::CreateDefaultScript()
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

FString FTIScriptOrchestrator::GetConfigDirectory()
{
	return UConfigManager::GetConfigurationFolderPath().Append("/TweakIt/");
}

FTIScriptOrchestrator* FTIScriptOrchestrator::Get()
{
	return FModuleManager::GetModuleChecked<FTweakItModule>("TweakIt").Orchestrator;
}

FScriptState FTIScriptOrchestrator::StartScript(FString Name)
{
	LOGF("Starting script \"%s\"", *Name)
	FString Path = GetConfigDirectory() + Name;
	if (!FPaths::FileExists(Path))
	{
		FScriptState Error = FScriptState::Errored;
		Error.Payload = "File does not exist";
		return Error;
	}
	FScript* Script = new FScript(Path);
	FScriptState State = Script->Start();
	if (State.IsCompleted())
	{
		delete Script;
	}
	RunningScripts.Emplace(Script);
	return State;
}

TArray<FString> FTIScriptOrchestrator::GetAllScripts()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	TArray<FString> Filenames;
	Manager.FindFiles(Filenames, *ConfigDirectory, TEXT(".lua"));
	return Filenames;
}
