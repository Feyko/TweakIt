#include "LuaFDelegate.h"
#include <string>

#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"
using namespace std;

int FLuaFDelegate::Construct(lua_State* L, UFunction* SignatureFunction, FScriptDelegate* Delegate)
{
	if (!SignatureFunction->IsValidLowLevel() || !Delegate)
	{
		LOG("Trying to construct a LuaFDelegate from an invalid signature function or delegate")
		lua_pushnil(L);
		return 1;
	}
	LOG("Constructing a LuaFDelegate")
	FLuaFDelegate* ReturnedInstance = static_cast<FLuaFDelegate*>(lua_newuserdata(L, sizeof(FLuaFDelegate)));
	new(ReturnedInstance) FLuaFDelegate{SignatureFunction, Delegate};
	luaL_getmetatable(L, FLuaFDelegate::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaFDelegate* FLuaFDelegate::Get(lua_State* L, int Index)
{
	return static_cast<FLuaFDelegate*>(luaL_checkudata(L, Index, Name));
}

FString FLuaFDelegate::ToString() const
{
	return Delegate->ToString<UObject>();
}

int FLuaFDelegate::Lua_Bind(lua_State* L)
{
	LOG("Binding")
    FLuaFDelegate* Self = Get(L);
	luaT_CheckLuaFunction(L, 2);
	UFunction* Function;
	UE4CodeGen_Private::FFunctionParams Params = UE4CodeGen_Private::FFunctionParams();
	LOG("Setting class name")
	Params.OwningClassName = TCHAR_TO_UTF8(*UTIUFunctionBinder::StaticClass()->GetName());
	LOG("Setting name")
	Params.NameUTF8 = TCHAR_TO_UTF8(*UTIUFunctionBinder::MakeFunctionName(FTILog::CurrentScript, Self->SignatureFunction->GetName()));
	LOG("Constructing")
	ConstructUFunction(Function, Params);
	LOG("Setting func")
	LOG(Function->IsValidLowLevel())
	LOG(Function->GetFullName())
	Function->SetNativeFunc([](UObject* Context, FFrame& TheStack, RESULT_DECL)
	{
		LOG("BROOOOO I CAN'T BELIEVE IT BRO")
	});
	LOG("Adding function")
	FString FunctionName = UTIUFunctionBinder::AddFunction(Function, FTILog::CurrentScript, Self->SignatureFunction->GetName());
	LOG("Binding function")
	LOG(Self->Delegate->GetUObject()->GetFullName())
	Self->Delegate->BindUFunction(UTIUFunctionBinder::Get(), FName(FunctionName));
	LOG("Finished")
	return 0;
}

int FLuaFDelegate::Lua_IsBound(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	lua_pushboolean(L, Self->Delegate->IsBound());
	return 1;
}

int FLuaFDelegate::Lua_Unbind(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	Self->Delegate->Unbind();
	return 0;
}

int FLuaFDelegate::Lua_Wait(lua_State* L)
{

	return 0;
}

int FLuaFDelegate::Lua_Trigger(lua_State* L)
{
	luaL_error(L, "Trigger is WIP");
	return 0;
}

int FLuaFDelegate::Lua__index(lua_State* L)
{
	FLuaFDelegate* Self = Get(L);
	const FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaFDelegate %s with %s", *Self->ToString(),*Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	return 0;
}

int FLuaFDelegate::Lua__call(lua_State* L)
{
	Lua_Trigger(L);
	return 0;
}

int FLuaFDelegate::Lua__tostring(lua_State* L)
{
	FLuaFDelegate* Self = static_cast<FLuaFDelegate*>(lua_touserdata(L, 1));
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Delegate->ToString<UObject>()));
	return 1;
}

void FLuaFDelegate::RegisterMetadata(lua_State* L)
{
	RegisterMetatable(L, Name, Metadata);
}
