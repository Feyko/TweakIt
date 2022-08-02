#include "TIScriptOrchestrator.h"

#include "FGGameInstance.h"
#include "TweakIt/Lua/Lua.h"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "Module/ModModule.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/Scripting/Script.h"

FTIScriptOrchestrator::FTIScriptOrchestrator()
{
	if(GetAllScripts().Num() == 0)
	{
		LOGFL("No scripts were found in %s. Creating the default script", Warning, *FPaths::ConvertRelativePathToFull(GetConfigDirectory()));
		CreateDefaultScript();
	}
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
	FArchive* file = Manager.CreateFileWriter(*FPaths::Combine(ConfigDirectory, TEXT("script.lua")));
	file->Logf(TEXT("print('Hello World!')"));
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
	return FPaths::Combine(UConfigManager::GetConfigurationFolderPath(), TEXT("TweakIt/"));
}

FTIScriptOrchestrator* FTIScriptOrchestrator::Get()
{
	return FModuleManager::GetModuleChecked<FTweakItModule>("TweakIt").Orchestrator;
}

FScriptState FTIScriptOrchestrator::StartScript(FString Name)
{
	LOGF("Starting script \"%s\"", *Name)
	FString Path = FPaths::Combine(GetConfigDirectory(), Name);
	if (!FPaths::FileExists(Path))
	{
		FScriptState Error = FScriptState::Errored;
		Error.Payload = "File does not exist";
		return Error;
	}
	FScript* Script = new FScript(Path);
	FScriptState State = Script->Start();
	CheckAfterScriptStop(Script);
	return State;
}

FScriptState FTIScriptOrchestrator::ResumeScript(FScript* Script)
{
	FScriptState State = Script->Resume();
	CheckAfterScriptStop(Script);
	return State;
}

void FTIScriptOrchestrator::CheckAfterScriptStop(FScript* Script)
{
	if (Script->GetState().IsCompleted())
	{
		delete Script;
	} else
	{
		RunningScripts.Emplace(Script);
		FEvent* Event = Script->L.PlatformEventWaitedFor;
		if (Event != nullptr)
		{
			FEvent* ReadyCallback = FPlatformProcess::CreateSynchEvent();
			AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [Event, Script, this, ReadyCallback]
			{
				ReadyCallback->Trigger();
				Event->Wait();
				AsyncTask(ENamedThreads::GameThread, [Script, this]
				{
					delete Script->L.PlatformEventWaitedFor;
					Script->L.PlatformEventWaitedFor = nullptr;
					ResumeScript(Script);
				});
			});
			ReadyCallback->Wait();
		}
	}
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
	Array.Append(init, UE_ARRAY_COUNT(init));
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
	// LOG(Event)
	if (Unique)
	{
		PassedUniqueEvents.Emplace(Event);
	}
	bool OK = true;
	for (auto Script : RunningScripts)
	{
		if (Script->L.EventWaitedFor == Event)
		{
			Script->L.EventWaitedFor = "";
			FScriptState State = ResumeScript(Script);
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
