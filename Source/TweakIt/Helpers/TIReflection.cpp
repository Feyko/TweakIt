#include "TIReflection.h"


#include "Engine/SCS_Node.h"
#include "TweakIt/TweakItModule.h"

UProperty* FTIReflection::FindPropertyByName(UStruct* Class, const TCHAR* PropertyName) {
	FString lowered = FString(PropertyName).ToLower();
	for (UProperty* Property = Class->PropertyLink; Property; Property = Property->PropertyLinkNext) {
		if (Property->GetName().ToLower() == lowered) {
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

UStruct* FTIReflection::FindStructUnreliable(FString ClassName) {
	for (TObjectIterator<UStruct> it; it; ++it) {
		UStruct* ItClass = *it;
		LOGFS(ItClass->GetFullName())
		if (ItClass->GetName().ToLower() == ClassName.ToLower()) {
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

UStruct* FTIReflection::FindStructByName(FString ClassName, FString Package = "FactoryGame") {
	LOGF("Trying to load %s", *ClassName)
    UStruct* LoadedClass = LoadObject<UStruct>(nullptr, *ClassName);
	if (!LoadedClass->IsValidLowLevel()) {
		LOGF("Trying to load /Script/%s.%s", *Package, *ClassName)
        LoadedClass = LoadObject<UStruct>(NULL, *("/Script/" + Package + "." + ClassName));
		if (!LoadedClass->IsValidLowLevel()) {
			LOG("Trying to find the class by iterating")
            LoadedClass = FindStructUnreliable(ClassName);
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
	TArray<UActorComponent*> FoundComponents;
	ActorCDO->GetComponents(InComponentClass,FoundComponents);

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
UClass* FTIReflection::GenerateUniqueSimpleClass(const TCHAR* PackageName, const TCHAR* ClassName, UClass* ParentClass) {
	LOGF("Creating subclass for %s", *ParentClass->GetFullName())
	LOGF("%ls", *(FString(PackageName) + "." + ClassName))
	UClass* LoadedClass = LoadObject<UClass>(nullptr, *(FString(PackageName) + "." + ClassName));
	LOGF("%hhd", LoadedClass->IsValidLowLevel())
	if(LoadedClass->IsValidLowLevel()) {LOG("A class with this path already exists") return LoadedClass;}
	//Flags to assign to newly created class
	const EClassFlags ParamsClassFlags = CLASS_MatchedSerializers;
	//Code below is taken from GetPrivateStaticClassBody
	//Allocate memory from ObjectAllocator for class object and call class constructor directly
	// ConstructedClassObject = ConstructedBPClassObject;
	// ConstructedClassObject->SetSuperStruct(ParentBPGC);
	UClass* ConstructedClassObject = nullptr;
	// if(UBlueprintGeneratedClass* ParentBPGC = Cast<UBlueprintGeneratedClass>(ParentClass)) {
	// 	LOG("Creating BPGC")
	// 	ConstructedClassObject = static_cast<UBlueprintGeneratedClass*>(
	// 		StaticConstructObject_Internal(ParentBPGC,
	// 		ParentBPGC,
	// 		ClassName,
	// 		RF_NoFlags,
	// 		EInternalObjectFlags::None,
	// 		ParentClass->GetDefaultObject(),
	// 		false,
	// 		nullptr));
	// 	ConstructedClassObject->ClassAddReferencedObjects = ParentClass->ClassAddReferencedObjects;
	// } else {
		LOG("Creating UCLASS")
		ConstructedClassObject = (UClass*)GUObjectAllocator.AllocateUObject(sizeof(UClass), alignof(UClass), true);
		::new(ConstructedClassObject)UClass(
            EC_StaticConstructor,
            ClassName,
            ParentClass->GetStructureSize(),
            ParentClass->GetMinAlignment(),
            CLASS_Intrinsic,
            CASTCLASS_None,
            UObject::StaticConfigName(),
            EObjectFlags(RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet),
            ParentClass->ClassConstructor,
            ParentClass->ClassVTableHelperCtorCaller,
            ParentClass->ClassAddReferencedObjects);
            ConstructedClassObject->SetSuperStruct(ParentClass);
	// }

	LOG("out")
	//Set super structure and ClassWithin (they are required prior to registering)
	ConstructedClassObject->ClassFlags |= (ParentClass->ClassFlags & CLASS_Inherit);
	ConstructedClassObject->ClassFlags ^= CLASS_Native;
	ConstructedClassObject->ClassCastFlags |= ParentClass->ClassCastFlags;
	ConstructedClassObject->ClassWithin = UObject::StaticClass();
	LOG("Flags")
	//Register pending object, apply class flags, set static type info and link it
	ConstructedClassObject->RegisterDependencies();
	LOG("Deps")
	if(!ConstructedClassObject->GetClass()) {
		LOG("Registering")
		ConstructedClassObject->DeferredRegister(UClass::StaticClass(), PackageName, ClassName);
	}
	LOG("Out")
	//Mark class as Constructed and perform linking
	ConstructedClassObject->ClassFlags |= (EClassFlags)(ParamsClassFlags | CLASS_Constructed);
	LOG("assembling")
	ConstructedClassObject->AssembleReferenceTokenStream(true);
	LOG("linking")
	ConstructedClassObject->StaticLink();
	//Make sure default class object is initialized and copy the values from parent CDO to handle values set by Blueprints
	LOG("getting CDO")
	UObject* CDO = ConstructedClassObject->GetDefaultObject();
	LOG("getting parent cdo")
	UObject* ParentCDO = ParentClass->GetDefaultObject();
	LOG("copying properties")
	for(UProperty* Property = ConstructedClassObject->PropertyLink;Property;Property = Property->PropertyLinkNext) {
		Property->CopyCompleteValue_InContainer(CDO, ParentCDO);
	}
	
	if(UBlueprintGeneratedClass* ClassAsBPGC = Cast<UBlueprintGeneratedClass>(ConstructedClassObject)) {
		LOG("Dumping Construction Script")
		for(USCS_Node* Node : ClassAsBPGC->SimpleConstructionScript->GetAllNodes()) {
			LOGFS(Node->ComponentClass->GetName())
		}
	}

	LOGFS(ConstructedClassObject->GetFullName())	
	return ConstructedClassObject;
}

void* FTIReflection::MakeStructInstance(UStruct* Struct) {
	void* instance = malloc(Struct->GetStructureSize());
	Struct->InitializeStruct(instance);
	return instance;
}