#include "TIUFunctionBinder.h"

#include "TweakIt/Logging/FTILog.h"

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
	static UTIUFunctionBinder* Self = NewObject<UTIUFunctionBinder>(GetTransientPackage(), "TweakItFunctionBinder", RF_MarkAsRootSet);
	return Self;
}

template<typename... T>
FString UTIUFunctionBinder::MakeFunctionName(T... Namespace)
{
	return FPaths::Combine(Namespace...);
}
