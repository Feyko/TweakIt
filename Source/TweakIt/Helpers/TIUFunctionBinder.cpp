#include "TIUFunctionBinder.h"

#include "TweakIt/Logging/FTILog.h"

UFunction* UTIUFunctionBinder::SignatureBuffer = nullptr;
TMap<FGuid, FEvent*> UTIUFunctionBinder::Awaitables = {};

template<typename... T>
FString UTIUFunctionBinder::AddNativeFunction(FNativeFuncPtr Function, T... Namespace)
{
	FString Name = MakeFunctionName(Namespace ...);
	UFunction* UFunc = nullptr;
	UE4CodeGen_Private::FFunctionParams Params = UE4CodeGen_Private::FFunctionParams();
	Params.OwningClassName = TCHAR_TO_UTF8(*StaticClass()->GetName());
	Params.NameUTF8 = TCHAR_TO_UTF8(*Name);
	Params.OuterFunc = []()->UObject*{return StaticClass();};
	ConstructUFunction(UFunc, Params);
	UFunc->SetNativeFunc(Function);
	StaticClass()->AddFunctionToFunctionMap(UFunc, FName(Name));
	return Name;
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
	FGuid Guid = FGuid();
	FEvent* Event = FPlatformProcess::CreateSynchEvent();
	Awaitables.Add(Guid, Event);
	Get()->AddNativeFunction([](UObject* Object, FFrame& Frame, void* Result)
	{
		FGuid Guid = FGuid();
		FGuid::Parse(FPaths::GetPathLeaf(Frame.Node->GetName()), Guid);
		FEvent** Event = Get()->Awaitables.Find(Guid);
		if (Event == nullptr)
		{
			LOGL("Tried to trigger unknown event", Error)
		}
		(*Event)->Trigger();
	}, "Awaitable", Guid.ToString());
	FunctionNameOut = FName(MakeFunctionName("Awaitable", Guid.ToString()));
	return Event;
}

template<typename... T>
FString UTIUFunctionBinder::MakeFunctionName(T... Namespace)
{
	return FPaths::Combine(Namespace...);
}
