#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

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
};
