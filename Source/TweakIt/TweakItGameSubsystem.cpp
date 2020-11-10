#include "TweakItGameSubsystem.h"


#include "FileManagerGeneric.h"
#include "Reflection.h"
#include "TIRunScriptCommand.h"
#include "Lua/Lua.h"
#include "mod/hooking.h"
using namespace TweakIt::Lua;

void UTweakItGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	RunAllScripts();
}

void UTweakItGameSubsystem::Deinitialize()
{
}

UTweakItGameSubsystem* UTweakItGameSubsystem::Get(UObject* WorldContext)
{
	if(!WorldContext->IsValidLowLevel()) {return nullptr;}
	return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem<UTweakItGameSubsystem>();
}


void UTweakItGameSubsystem::InitialiseLuaState()
{
	if(LuaState) {return;}
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

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
	
	lua_register(L, "GetClass", lua_GetClass);
	lua_register(L, "LoadObject", lua_LoadObject);
	lua_register(L, "Log", lua_Print);
	LuaState = L;
}

void UTweakItGameSubsystem::RunAllScripts()
{
	LOG("[TweakIt] Running all scripts")
	IFileManager& manager = FFileManagerGeneric::Get();
	FString configDir = SML::GetConfigDirectory().Append("/TweakIt/");
	LOGFS(configDir)
	if(!manager.DirectoryExists(*configDir))
	{
		LOG("[TweakIt] Creating the default directory and script")
		manager.MakeDirectory(*configDir);
		FArchive* file = manager.CreateFileWriter(*configDir.Append("script.lua"));
		file->Close();
	}
	TArray<FString> filenames;
	manager.FindFiles(filenames, *configDir, TEXT(".lua"));
	for(FString& filename : filenames)
	{
		FString PathPart;
		FString FilenamePart;
		FString ExtensionPart;
		LOGFS(filename);
		RunScript(filename);
	}
}

bool UTweakItGameSubsystem::RunScript(FString name)
{
	InitialiseLuaState();
	LOGFS(FString::Printf(TEXT("[TweakIt] Running script \"%s\""), *name))
	FString path = SML::GetConfigDirectory() + FString("/TweakIt/") + name;
	if(!FPaths::FileExists(path)) {return false;}
	return CheckLua(LuaState, luaL_dofile(LuaState, TCHAR_TO_UTF8(*path)));
}

bool UTweakItGameSubsystem::RegisterCommands(UObject* WorldContext)
{
	LOG("[TweakIt] Registering the commands")
	AChatCommandSubsystem* Subsystem = AChatCommandSubsystem::Get(WorldContext);
	if(!Subsystem->IsValidLowLevel())
	{
		LOG("The Chat Command Subsystem was invalid")
		return false;
	}
	Subsystem->RegisterCommand(ATIRunScriptCommand::StaticClass());
	return true;
}