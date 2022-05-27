#include "TweakItSubsystem.h"

#include <Subsystem/SubsystemActorManager.h>

#include "FGGameInstance.h"
#include "TweakIt/Lua/lib/lua.hpp"
#include "TweakIt/Commands/TIRunScriptCommand.h"
#include "Configuration/ConfigManager.h"
#include "HAL/FileManagerGeneric.h"
#include "TweakIt/Lua/Lua.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/Lua/LuaTArray.h"
#include "TweakIt/Lua/LuaUStruct.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Lua/LuaUClass.h"
#include "TweakIt/Lua/LuaUObject.h"

void ATweakItSubsystem::BeginPlay() {
	LOG("TweakIt Version 0.4.0-dev is now loaded")
	InitialiseLuaState();
	RunAllScripts();
}


ATweakItSubsystem* ATweakItSubsystem::Get(UObject* WorldContext) {
	if (!WorldContext->IsValidLowLevel()) { return nullptr; }
	return WorldContext->GetWorld()->GetSubsystem<USubsystemActorManager>()->GetSubsystemActor<ATweakItSubsystem>();
}

void ATweakItSubsystem::RegisterMetadatas(lua_State* L)
{
	LuaUClass::RegisterMetadata(L);
	LuaUObject::RegisterMetadata(L);
	LuaTArray::RegisterMetadata(L);
	LuaUStruct::RegisterMetadata(L);
}

void ATweakItSubsystem::RegisterGlobalFunctions(lua_State* L) {
	lua_register(L, "GetClass", lua_GetClass);
	lua_register(L, "LoadObject", lua_LoadObject);
	lua_register(L, "UnlockRecipe", lua_UnlockRecipe);
	lua_register(L, "Log", lua_Print);
	lua_register(L, "print", lua_Print);
	lua_register(L, "MakeSubclass", lua_MakeSubclass);
	lua_register(L, "MakeStructInstance", lua_MakeStructInstance);
}

void ATweakItSubsystem::InitialiseLuaState() {
	if (LuaState) return;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	RegisterMetadatas(L);
	RegisterGlobalFunctions(L);
	if(this->IsA(AActor::StaticClass())) {
		LOG("This is an Actor")
	}
	LuaUObject::ConstructObject(L, this);
	lua_setglobal(L, "WorldContext");
	lua_getglobal(L, "WorldContext");
	LuaUObject* self = (LuaUObject*)lua_touserdata(L, -1);
	if(self->Object->IsA(AActor::StaticClass())) {
		LOG("Self is an Actor")
    }
	LuaState = L;
	
}

bool ATweakItSubsystem::RunAllScripts() {
	LOG("[TweakIt] Running all scripts")
	IFileManager& manager = FFileManagerGeneric::Get();
	FString configDir = UConfigManager::GetConfigurationFolderPath().Append("/TweakIt/");
	if (!manager.DirectoryExists(*configDir)) {
		LOG("[TweakIt] Creating the default directory and script")
		manager.MakeDirectory(*configDir);
		FArchive* file = manager.CreateFileWriter(*configDir.Append("script.lua"));
		file->Close();
	}
	TArray<FString> filenames;
	manager.FindFiles(filenames, *configDir, TEXT(".lua"));
	for (FString& filename : filenames) {
		FString PathPart;
		FString FilenamePart;
		FString ExtensionPart;
		LOGFS(filename);
		bool OK = RunScript(filename);
		if (!OK)
		{
			return false;
		}
	}
	return true;
}

bool ATweakItSubsystem::RunScript(FString name) {
	LOGFS(FString::Printf(TEXT("[TweakIt] Running script \"%s\""), *name))
	FString path = UConfigManager::GetConfigurationFolderPath() + FString("/TweakIt/") + name;
	if (!FPaths::FileExists(path)) { return false; }
	return CheckLua(LuaState, luaL_dofile(LuaState, TCHAR_TO_UTF8(*path)));
}