#include "TIScriptOrchestrator.h"

#include "FGGameInstance.h"
#include "TweakIt/Lua/Lua.h"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "Interfaces/IPluginManager.h"
#include "ModLoading/ModLoadingLibrary.h"
#include "Module/ModModule.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/Scripting/Script.h"

FTIScriptOrchestrator::FTIScriptOrchestrator()
{
	CreateDefaultScript();
	SetupModEvents();
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

void FTIScriptOrchestrator::SetupModEvents()
{
	FModuleManager::Get().OnModulesChanged().AddLambda([this](FName ModuleName, EModuleChangeReason Reason)
	{
		if (Reason == EModuleChangeReason::ModuleLoaded)
		{
			ResumeForMod(ModuleName.ToString());
		}
	});

	UModModule* Module = const_cast<UModModule*>(GetDefault<UModModule>());
	SUBSCRIBE_METHOD_VIRTUAL_AFTER(UModModule::DispatchLifecycleEvent, Module, [this](UModModule* Self, ELifecyclePhase Phase)
	{
		FString PhaseString = StaticEnum<ELifecyclePhase>()->GetNameStringByValue(int64(Phase));
		ResumeForMod(Self->GetOwnerModReference().ToString(), PhaseString);
	})
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
	} else
	{
		RunningScripts.Emplace(Script);
	}
	return State;
}

FString FTIScriptOrchestrator::MakeEventForMod(FString ModReference, FString Lifecycle)
{
	return MakeEventString("Mod", ModReference, Lifecycle);
}

// TODO: Make lifecycle events non-unique
bool FTIScriptOrchestrator::ResumeForMod(FString ModReference, FString Lifecycle /* = "Module"*/)
{
	FString Event = MakeEventForMod(ModReference, Lifecycle);
	return ResumeScriptsWaitingForEvent(true, Event);
}

template <typename ... T>
FString FTIScriptOrchestrator::MakeEventString(T... EventParts)
{
	FString init[] = {EventParts...};
	TArray<FString> Array;
	Array.Append(init, ARRAY_COUNT(init));
	return FString::Join(Array, TEXT(":"));
}

template <typename ... T>
bool FTIScriptOrchestrator::HasEventPassed(T... EventParts)
{
	FString Event = MakeEventString(EventParts...);
	return PassedUniqueEvents.Find(Event) != INDEX_NONE;
}

bool FTIScriptOrchestrator::HasModPassed(FString ModReference, FString Lifecycle)
{
	return HasEventPassed(MakeEventForMod(ModReference, Lifecycle));
}

template <typename ... T>
bool FTIScriptOrchestrator::ResumeScriptsWaitingForEvent(bool Unique, T... EventParts)
{
	FString Event = MakeEventString(EventParts...);
	LOG(Event)
	if (Unique)
	{
		PassedUniqueEvents.Emplace(Event);
	}
	bool OK = true;
	for (auto Script : RunningScripts)
	{
		if (Script->L.EventWaitedFor == Event)
		{
			FScriptState State = Script->Resume();
			if (State == FScriptState::Errored)
			{
				OK = false;
			}
		}
	}
	return OK;
}

TArray<FString> FTIScriptOrchestrator::GetAllScripts()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	TArray<FString> Filenames;
	Manager.FindFiles(Filenames, *ConfigDirectory, TEXT(".lua"));
	return Filenames;
}