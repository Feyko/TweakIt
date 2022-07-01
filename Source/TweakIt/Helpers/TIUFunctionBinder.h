#pragma once
#include "TIUFunctionBinder.generated.h"

UCLASS()
class UTIUFunctionBinder : public UObject
{
	GENERATED_BODY()
public:
	template <class ... T>
	FString AddNativeFunction(FNativeFuncPtr Function, T ... Namespace);
	template<typename... T>
	static FString AddFunction(UFunction* Function, T... Namespace);
	
	template<typename... T>
	static UFunction* GetFunction(T... Namespace);
	
	template<typename... T>
	static void RemoveFunction(T... Namespace);
	
	static UTIUFunctionBinder* Get();
	
	template<typename... T>
	static FString MakeFunctionName(T... Namespace);

	static UFunction* SignatureBuffer;
};
