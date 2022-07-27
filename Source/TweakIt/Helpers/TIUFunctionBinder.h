#pragma once
#include "TIUFunctionBinder.generated.h"

UCLASS()
class UTIUFunctionBinder : public UObject
{
	GENERATED_BODY()
public:
	static void AddNativeFunction(FNativeFuncPtr Function, FName Name);
	
	template<typename... T>
	static FString AddFunction(UFunction* Function, T... Namespace);
	
	template<typename... T>
	static UFunction* GetFunction(T... Namespace);
	
	template<typename... T>
	static void RemoveFunction(T... Namespace);
	
	static UTIUFunctionBinder* Get();
	static FEvent* MakeAwaitableFunction(FName& FunctionNameOut);

	template<typename... T>
	static FString MakeFunctionName(T... Namespace);

	static UFunction* SignatureBuffer;
	static TMap<FGuid, FEvent*> Awaitables;
};
