#include "TIUFunctionBinder.h"

#include "TweakIt/Logging/FTILog.h"

UFunction* UTIUFunctionBinder::SignatureBuffer = nullptr;
TMap<FGuid, FEvent*> UTIUFunctionBinder::Awaitables = {};

void UTIUFunctionBinder::AddNativeFunction(FNativeFuncPtr Function, FName Name)
{
	LOG("Adding a native function")
	UFunction* UFunc = nullptr;
	UE4CodeGen_Private::FFunctionParams Params = UE4CodeGen_Private::FFunctionParams();
	Params.OwningClassName = TCHAR_TO_UTF8(*StaticClass()->GetName());
	Params.NameUTF8 = TCHAR_TO_UTF8(*Name.ToString());
	Params.OuterFunc = []()->UObject*{return StaticClass();};
	Params.FunctionFlags = FUNC_Native|FUNC_Public;
	ConstructUFunction(UFunc, Params);
	UFunc->SetNativeFunc(Function);
	StaticClass()->AddFunctionToFunctionMap(UFunc, Name);
}

template<typename... T>
FString UTIUFunctionBinder::AddFunction(UFunction* Function, T... Namespace)
{
	FString Name = MakeFunctionName(Namespace ...);
	StaticClass()->AddFunctionToFunctionMap(Function, FName(Name));
	return Name;
}

template<typename... T>
UFunction* UTIUFunctionBinder::GetFunction(T... Namespace)
{
	FString Name = MakeFunctionName(Namespace...);
	return StaticClass()->FindFunctionByName(FName(Name));
}

template<typename... T>
void UTIUFunctionBinder::RemoveFunction(T... Namespace)
{
	StaticClass()->RemoveFunctionFromFunctionMap(GetFunction(Namespace...));
}

UTIUFunctionBinder* UTIUFunctionBinder::Get()
{
	return Cast<UTIUFunctionBinder>(StaticClass()->ClassDefaultObject);
}

// TODO: Make this non-destructive
// TODO: Make sure the Event won't block or crash when triggering multiple times without waiting
FEvent* UTIUFunctionBinder::MakeAwaitableFunction(FName& FunctionNameOut)
{
	FGuid Guid = FGuid::NewGuid();
	FEvent* Event = FPlatformProcess::CreateSynchEvent();
	Awaitables.Add(Guid, Event);
	FName FunctionName = FName(MakeFunctionName(TEXT("Awaitable"), Guid.ToString()));
	AddNativeFunction([](UObject* Object, FFrame& Frame, void* Result)
	{
		FGuid Guid = FGuid();
		FGuid::Parse(FPaths::GetPathLeaf(Frame.Node->GetName()), Guid);
		FEvent** Event = Get()->Awaitables.Find(Guid);
		if (Event == nullptr)
		{
			LOGL("Tried to trigger unknown event", Error)
		}
		(*Event)->Trigger();
	}, FunctionName);
	FunctionNameOut = FunctionName;
	return Event;
}

template<typename... T>
FString UTIUFunctionBinder::MakeFunctionName(T... Namespace)
{
	return FPaths::Combine(Namespace...);
}
