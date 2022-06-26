#include "TIUFunctionBinder.h"

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
