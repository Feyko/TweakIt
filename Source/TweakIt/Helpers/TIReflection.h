#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

using namespace UE4CodeGen_Private;

class FTIReflection
{
public:
	static UActorComponent* FindDefaultComponentByName(
		UClass* InActorClass,
		const TSubclassOf<UActorComponent> InComponentClass,
		FString ComponentName
	);
	static UClass* FindBPUnreliable(FString ClassName);
	static UStruct* FindStructUnreliable(FString ClassName);
	static UClass* FindClassByName(FString ClassName, FString Package);
	static UStruct* FindStructByName(FString ClassName, FString Package);
	static UProperty* FindPropertyByName(UStruct* Class, const TCHAR* PropertyName);

	static UClass* GenerateUniqueSimpleClass(const TCHAR* PackageName, const TCHAR* ClassName, UClass* ParentClass);
	static void* MakeStructInstance(UStruct* Struct);
	static void* CopyStruct(UStruct* Struct, void* Values);

	static UFunction* CopyUFunction(UFunction* ToCopy, FString FunctionName, UClass* Outer = nullptr);
	static FProperty* CopyProperty(FFieldVariant Outer, FProperty* Prop);

	static uint8 GetBoolPropertyBitmask(FBoolProperty* Prop);
	static void ReverseChildProperties(FField** Head);
	template <class T>
	static int AddValueToEnum(FName NewName);
private:
	static UClass* UFunctionOuterBuffer;
};
