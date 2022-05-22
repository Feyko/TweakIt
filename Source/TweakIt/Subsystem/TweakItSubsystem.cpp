#include "TweakItSubsystem.h"

#include <Subsystem/SubsystemActorManager.h>

#include "SatisfactoryModLoader.h"
#include "FGGameInstance.h"
#include "FGGameMode.h"
#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Commands/TIRunScriptCommand.h"
#include "Configuration/ConfigManager.h"
#include "Configuration/ModConfiguration.h"
#include "HAL/FileManagerGeneric.h"
#include "TweakIt/Lua/Lua.h"
#include "SML/Public/Patching/NativeHookManager.h"
#include "TweakIt/Lua/LuaTArray.h"
#include "TweakIt/Lua/LuaUStruct.h"
#include "TweakIt/TweakItModule.h"
#include "TweakIt/Lua/LuaUClass.h"
#include "TweakIt/Lua/LuaUObject.h"
using namespace TweakIt::Lua;

void ATweakItSubsystem::BeginPlay() {
	LOG("TweakIt Version 0.4.0-dev is now loaded")
	InitialiseLuaState();
	RegisterCommands();
	RunAllScripts();
}


ATweakItSubsystem* ATweakItSubsystem::Get(UObject* WorldContext) {
	if (!WorldContext->IsValidLowLevel()) { return nullptr; }
	return WorldContext->GetWorld()->GetSubsystem<USubsystemActorManager>()->GetSubsystemActor<ATweakItSubsystem>();
}

void ATweakItSubsystem::RegisterLuaUClassMetadata(lua_State* L) {
	luaL_newmetatable(L, "LuaUClassMeTa");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, LuaUClass::lua_index);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, LuaUClass::lua_newindex);
	lua_settable(L, -3);
	lua_pushstring(L, "__call");
	lua_pushcfunction(L, LuaUClass::lua__call);
	lua_settable(L, -3);
	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, LuaUClass::lua__tostring);
	lua_settable(L, -3);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, LuaUClass::lua_gc);
	lua_settable(L, -3);
}

void ATweakItSubsystem::RegisterLuaUObjectMetadata(lua_State* L) {
	luaL_newmetatable(L, "LuaUObjectMeTa");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, LuaUObject::lua_index);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, LuaUObject::lua_newindex);
	lua_settable(L, -3);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, LuaUObject::lua_gc);
	lua_settable(L, -3);
	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, LuaUObject::lua__tostring);
	lua_settable(L, -3);
}

void ATweakItSubsystem::RegisterLuaTArrayMetadata(lua_State* L) {
	luaL_newmetatable(L, "LuaTArrayMeTa");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, LuaTArray::lua_index);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, LuaTArray::lua_newindex);
	lua_settable(L, -3);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, LuaTArray::lua_gc);
	lua_settable(L, -3);
	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, LuaTArray::lua__tostring);
	lua_settable(L, -3);
}

void ATweakItSubsystem::RegisterLuaUStructMetadata(lua_State* L) {
	luaL_newmetatable(L, "LuaUStructMeTa");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, LuaUStruct::lua_index);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, LuaUStruct::lua_newindex);
	lua_settable(L, -3);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, LuaUStruct::lua_gc);
	lua_settable(L, -3);
	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, LuaUStruct::lua__tostring);
	lua_settable(L, -3);
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
	RegisterLuaUClassMetadata(L);
	RegisterLuaUObjectMetadata(L);
	RegisterLuaTArrayMetadata(L);
	RegisterLuaUStructMetadata(L);
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

void ATweakItSubsystem::RunAllScripts() {
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
		RunScript(filename);
	}
}

bool ATweakItSubsystem::RunScript(FString name) {
	LOGFS(FString::Printf(TEXT("[TweakIt] Running script \"%s\""), *name))
	FString path = UConfigManager::GetConfigurationFolderPath() + FString("/TweakIt/") + name;
	if (!FPaths::FileExists(path)) { return false; }
	return CheckLua(LuaState, luaL_dofile(LuaState, TCHAR_TO_UTF8(*path)));
}

bool ATweakItSubsystem::RegisterCommands() {
	LOG("[TweakIt] Registering the commands")
	AChatCommandSubsystem* Subsystem = AChatCommandSubsystem::Get(this);
	if (!Subsystem->IsValidLowLevel()) {
		LOG("The Chat Command Subsystem was invalid")
		return false;
	}
	Subsystem->RegisterCommand("TweakIt",ATIRunScriptCommand::StaticClass());
	return true;
}