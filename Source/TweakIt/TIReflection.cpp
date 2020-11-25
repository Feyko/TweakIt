#include "TIReflection.h"
#include "Lua/Lua.h"

UProperty* FTIReflection::FindPropertyByName(UStruct* Class, const TCHAR* PropertyName) {
	PropertyName = *FString(PropertyName).ToLower();
	for (UProperty* Property = Class->PropertyLink; Property; Property = Property->PropertyLinkNext) {
		if (Property->GetName().ToLower() == PropertyName) {
			return Property;
		}
	}
	return nullptr;
}

UClass* FTIReflection::FindBPUnreliable(FString ClassName) {
	for (TObjectIterator<UBlueprintGeneratedClass> it; it; ++it) {
		UClass* ItClass = *it;
		if (ItClass->GetName() == ClassName) {
			return ItClass;
		}
	}
	LOGF("Was unable to find the BP class \"%s\" by iterating", *ClassName)
	return nullptr;
}

UClass* FTIReflection::FindClassByName(FString ClassName, FString Package = "FactoryGame") {
	LOGF("Trying to load %s", *ClassName)
	UClass* LoadedClass = LoadObject<UClass>(nullptr, *ClassName);
	if (!LoadedClass->IsValidLowLevel()) {
		LOGF("Trying to load /Script/%s.%s", *Package, *ClassName)
        LoadedClass = LoadObject<UClass>(NULL, *("/Script/" + Package + "." + ClassName));
		if (!LoadedClass->IsValidLowLevel()) {
			LOG("Trying to find the class by iterating")
            LoadedClass = FindBPUnreliable(ClassName);
		}
	}
	return LoadedClass;
}

UActorComponent* FTIReflection::FindDefaultComponentByName(
	UClass* InActorClass,
	const TSubclassOf<UActorComponent> InComponentClass,
	FString ComponentName
) {
	LOG("FindDefaultComponentByName")
	FString GeneratedComponentName = ComponentName + "_GEN_VARIABLE";
	if (!IsValid(InActorClass)) {
		LOG("Not valid")
		return nullptr;
	}
	// Check CDO.
	AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();
	TArray<UActorComponent*> FoundComponents = ActorCDO->GetComponentsByClass(InComponentClass);

	for (auto Component : FoundComponents) {
		LOGFS(Component->GetName())
		if (Component->GetName() == ComponentName) {
			return Component;
		}
	}

	// Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from
	// CDO.
	UBlueprintGeneratedClass* RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);
	UClass* ActorClass = InActorClass;

	// Go down the inheritance tree to find nodes that were added to parent blueprints of our blueprint graph.
	do {
		LOGFS(ActorClass->GetName())
		UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
		if (!ActorBlueprintGeneratedClass) {
			LOG("not BlueprintGenerated")
			return nullptr;
		}

		const TArray<USCS_Node*>& ActorBlueprintNodes =
			ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

		for (USCS_Node* Node : ActorBlueprintNodes) {
			UClass* Class = Node->ComponentClass;
			LOGFS(Class->GetName())
			if (!Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->IsValidLowLevel()) { return nullptr; }
			if (Class->IsChildOf(InComponentClass) && Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->
															GetName() == GeneratedComponentName) {
				return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);
			}
		}

		ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());

	} while (ActorClass != AActor::StaticClass());

	return nullptr;
}

//Yoinked from upcoming SML update, I needed it
UClass* FTIReflection::GenerateSimpleClass(const TCHAR* PackageName, const TCHAR* ClassName, UClass* ParentClass) {
	//Flags to assign to newly created class
	const EClassFlags ParamsClassFlags = CLASS_Native | CLASS_MatchedSerializers;

	//Code below is taken from GetPrivateStaticClassBody
	//Allocate memory from ObjectAllocator for class object and call class constructor directly
	UClass* ConstructedClassObject = (UClass*)GUObjectAllocator.AllocateUObject(sizeof(UClass), alignof(UClass), true);
	::new(ConstructedClassObject)UClass(
		EC_StaticConstructor,
		ClassName,
		ParentClass->GetStructureSize(),
		CLASS_Intrinsic,
		CASTCLASS_None,
		UObject::StaticConfigName(),
		EObjectFlags(RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet),
		ParentClass->ClassConstructor,
		ParentClass->ClassVTableHelperCtorCaller,
		ParentClass->ClassAddReferencedObjects);

	//Set super structure and ClassWithin (they are required prior to registering)
	FCppClassTypeInfoStatic TypeInfoStatic = {false};
	ConstructedClassObject->SetSuperStruct(ParentClass);
	ConstructedClassObject->ClassWithin = UObject::StaticClass();
	ConstructedClassObject->SetCppTypeInfoStatic(&TypeInfoStatic);

	//Register pending object, apply class flags, set static type info and link it
	ConstructedClassObject->RegisterDependencies();
	ConstructedClassObject->DeferredRegister(UClass::StaticClass(), PackageName, ClassName);

	//Mark class as Constructed and perform linking
	ConstructedClassObject->ClassFlags |= (EClassFlags)(ParamsClassFlags | CLASS_Constructed);
	ConstructedClassObject->AssembleReferenceTokenStream(true);
	ConstructedClassObject->StaticLink();

	//Make sure default class object is initialized
	ConstructedClassObject->GetDefaultObject();
	return ConstructedClassObject;
}
