#include "LuaFMulticastDelegate.h"

#include "LuaFDelegate.h"
#include <string>

#include "TweakIt/TweakItTesting.h"
#include "TweakIt/Helpers/TIReflection.h"
#include "TweakIt/Helpers/TIUFunctionBinder.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Lua/FTILuaFuncManager.h"
#include "TweakIt/Lua/LuaState.h"
using namespace std;

FLuaFMulticastDelegate::FLuaFMulticastDelegate(UFunction* Signature, FMulticastScriptDelegate* Delegate) : SignatureFunction(Signature), Delegate(Delegate)
{
	FMulticastDelegateProperty* Prop;
}

int FLuaFMulticastDelegate::Construct(lua_State* L, UFunction* SignatureFunction, FMulticastScriptDelegate* Delegate)
{
	if (!SignatureFunction->IsValidLowLevel() || !Delegate)
	{
		LOG("Trying to construct a LuaFDelegate from an invalid signature function or delegate")
		lua_pushnil(L);
		return 1;
	}
	LOG("Constructing a LuaFDelegate")
	FLuaFMulticastDelegate** ReturnedInstance = static_cast<FLuaFMulticastDelegate**>(lua_newuserdata(L, sizeof(FLuaFMulticastDelegate*)));
	*ReturnedInstance = new FLuaFMulticastDelegate(SignatureFunction, Delegate);
	luaL_getmetatable(L, FLuaFMulticastDelegate::Name);
	lua_setmetatable(L, -2);
	return 1;
}

FLuaFMulticastDelegate* FLuaFMulticastDelegate::Get(lua_State* L, int Index)
{
	return static_cast<FLuaFMulticastDelegate*>(luaL_checkudata(L, Index, Name));
}

void FLuaFMulticastDelegate::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(SignatureFunction);
}

FString FLuaFMulticastDelegate::ToString() const
{
	return Delegate->ToString<UObject>();
}

int FLuaFMulticastDelegate::Lua_Add(lua_State* L)
{
	LOG("Binding a LuaFDelegate")
    FLuaFMulticastDelegate* Self = Get(L);
	FTILua::LuaT_ExpectLuaFunction(L, 2);
	bool Unique = FTILua::LuaT_OptBoolean(L, 3, false);

	// TODO: Extract the following into function
	FString FunctionName = UTIUFunctionBinder::MakeFunctionName(FTILog::CurrentScript, Self->SignatureFunction->GetName());
	LOG("Copying UFunction")
	UFunction* Function = FTIReflection::CopyUFunction(Self->SignatureFunction, FunctionName);
	FunctionName = Function->GetFullName();
	LOG("Dumping Lua func")
	FTILuaFuncManager::DumpFunction(L, FunctionName, 2);
	LOG("Making native func from lua func")
	FNativeFuncPtr Func = FTILuaFuncManager::SavedLuaFuncToNativeFunc(L, FunctionName);
	LOG("Setting native func")
	Function->SetNativeFunc(Func);
	LOG("Adding function")
	UTIUFunctionBinder::AddFunction(Function, FunctionName);
	LOG("Binding function")
	// Self->Delegate->BindUFunction(UTIUFunctionBinder::Get(), FName(FunctionName));
	return 0;
}

int FLuaFMulticastDelegate::Lua_Remove(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	FLuaUFunction* Function = FLuaUFunction::Get(L, 2);
	Self->Delegate->Remove(Function->Object, Function->Function->GetFName());
	return 0;
}

int FLuaFMulticastDelegate::Lua_RemoveAll(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	FLuaUObject* Object = FLuaUObject::Get(L, 2);
	Self->Delegate->RemoveAll(Object->Object);
	return 0;
}

int FLuaFMulticastDelegate::Lua_Clear(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	Self->Delegate->Clear();
	return 0;
}

int FLuaFMulticastDelegate::Lua_Wait(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	FName FunctionName = FName(UTIUFunctionBinder::MakeFunctionName(""));
	LOG(FunctionName)
	FEvent* Event =  UTIUFunctionBinder::MakeAwaitableFunction(FunctionName);
	FScriptDelegate Delegate = FScriptDelegate();
	Delegate.BindUFunction(UTIUFunctionBinder::Get(), FunctionName);
	Self->Delegate->Add(Delegate);
	FLuaState::Get(L)->PlatformEventWaitedFor = Event;
	lua_yield(L, 0);
	return 0;
}

int FLuaFMulticastDelegate::Lua_Trigger(lua_State* L)
{
	LOG("Triggering LuaFDelegate")
	FLuaFMulticastDelegate* Self = Get(L);
	if (!Self->Delegate->IsBound())
	{
		LOGL("Tried to trigger unbound delegate", Warning)
		return 0;
	}
	// UObject* Object = Self->Delegate->ProcessMulticastDelegate<UObject>();
	// UFunction* Function = Object->FindFunction(Self->Delegate->GetFunctionName());
	// if (!Function->IsValidLowLevel())
	// {
	// 	LOGL("Tried to trigger delegate bound to an invalid UFunction", Warning)
	// 	return 0;
	// }
	// return FTILua::CallUFunction(L, Object, Function,2);
	return 0;
}

int FLuaFMulticastDelegate::Lua_Contains(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	FLuaUFunction* Function = FLuaUFunction::Get(L, 2);
	lua_pushboolean(L, Self->Delegate->Contains(Function->Object, Function->Function->GetFName()));
	return 1;
}

int FLuaFMulticastDelegate::Lua_IsBound(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	lua_pushboolean(L, Self->Delegate->IsBound());
	return 1;
}

int FLuaFMulticastDelegate::Lua__index(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	const FString Index = luaL_checkstring(L, 2);
	LOGF("Indexing a LuaFDelegate %s with %s", *Self->ToString(),*Index)
	if (lua_CFunction* Method = Methods.Find(Index))
	{
		lua_pushcfunction(L, *Method);
		return 1;
	}
	return 0;
}

int FLuaFMulticastDelegate::Lua__call(lua_State* L)
{
	Lua_Trigger(L);
	return 0;
}

int FLuaFMulticastDelegate::Lua__tostring(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	lua_pushstring(L, TCHAR_TO_UTF8(*Self->Delegate->ToString<UObject>()));
	return 1;
}

int FLuaFMulticastDelegate::Lua__gc(lua_State* L)
{
	FLuaFMulticastDelegate* Self = Get(L);
	delete Self;
	return 0;
}

void FLuaFMulticastDelegate::RegisterMetadata(lua_State* L)
{
	FTILua::RegisterMetatable(L, Name, Metadata);
}
