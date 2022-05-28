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
	LOG("TweakIt Version 0.6.0-dev is now loaded")
	CreateDefaultScript();
	InitialiseLuaState();
	RunAllScripts();
}


ATweakItSubsystem* ATweakItSubsystem::Get(UObject* WorldContext) {
	if (!WorldContext->IsValidLowLevel()) { return nullptr; }
	return WorldContext->GetWorld()->GetSubsystem<USubsystemActorManager>()->GetSubsystemActor<ATweakItSubsystem>();
}

void ATweakItSubsystem::InitialiseLuaState() {
	if (LuaState) return;
	lua_State* L = luaL_newstate();
	LuaState = L;
	luaL_openlibs(L);
	RegisterMetadatas(L);
	RegisterGlobalFunctions(L);
	RegisterWorldContext(L);
}

void ATweakItSubsystem::OpenLibs(lua_State* L)
{
	luaL_Reg Libs[] = {
		{"_G", luaopen_base},
		{LUA_LOADLIBNAME, luaopen_package},
		{LUA_COLIBNAME, luaopen_coroutine},
		{LUA_TABLIBNAME, luaopen_table},
		{LUA_STRLIBNAME, luaopen_string},
		{LUA_MATHLIBNAME, luaopen_math},
		{LUA_UTF8LIBNAME, luaopen_utf8},
		{LUA_DBLIBNAME, luaopen_debug},
	};
	for (auto Lib : Libs)
	{
		luaL_requiref(L, Lib.name, Lib.func, 1);
		lua_pop(L, 1);
	}
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

void ATweakItSubsystem::RegisterWorldContext(lua_State* L)
{
	LuaUObject::ConstructObject(L, this);
	lua_setglobal(L, "WorldContext");
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
	return CheckLua(LuaState, luaL_dofile(LuaState, TCHAR_TO_UTF8(*Path)));
}

TArray<FString> ATweakItSubsystem::GetAllScripts()
{
	IFileManager& Manager = FFileManagerGeneric::Get();
	FString ConfigDirectory = GetConfigDirectory();
	TArray<FString> Filenames;
	Manager.FindFiles(Filenames, *ConfigDirectory, TEXT(".lua"));
	return Filenames;
}
