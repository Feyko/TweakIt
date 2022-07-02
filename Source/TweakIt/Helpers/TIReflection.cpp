#include "TIReflection.h"

#include <functional>


#include "Engine/SCS_Node.h"
#include "TweakIt/Logging/FTILog.h"
#include "TweakIt/Logging/FTILog.h"

UProperty* FTIReflection::FindPropertyByName(UStruct* Class, const TCHAR* PropertyName)
{
	FString lowered = FString(PropertyName).ToLower();
	for (UProperty* Property = Class->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		if (Property->GetName().ToLower() == lowered)
		{
			return Property;
		}
	}
	return nullptr;
}

UClass* FTIReflection::FindBPUnreliable(FString ClassName)
{
	for (TObjectIterator<UBlueprintGeneratedClass> it; it; ++it)
	{
		UClass* ItClass = *it;
		if (ItClass->GetName() == ClassName)
		{
			return ItClass;
		}
	}
	LOGF("Was unable to find the BP class \"%s\" by iterating", *ClassName)
	return nullptr;
}

UStruct* FTIReflection::FindStructUnreliable(FString ClassName)
{
	for (TObjectIterator<UStruct> it; it; ++it)
	{
		UStruct* ItClass = *it;
		LOG(ItClass->GetFullName())
		if (ItClass->GetName().ToLower() == ClassName.ToLower())
		{
			return ItClass;
		}
	}
	LOGF("Was unable to find the BP class \"%s\" by iterating", *ClassName)
	return nullptr;
}

UClass* FTIReflection::FindClassByName(FString ClassName, FString Package = "FactoryGame")
{
	LOGF("Trying to load %s", *ClassName)
	UClass* LoadedClass = LoadObject<UClass>(nullptr, *ClassName);
	if (!LoadedClass->IsValidLowLevel())
	{
		LOGF("Trying to load /Script/%s.%s", *Package, *ClassName)
		LoadedClass = LoadObject<UClass>(nullptr, *("/Script/" + Package + "." + ClassName));
		if (!LoadedClass->IsValidLowLevel())
		{
			LOG("Trying to find the class by iterating")
			LoadedClass = FindBPUnreliable(ClassName);
		}
	}
	return LoadedClass;
}

UStruct* FTIReflection::FindStructByName(FString ClassName, FString Package = "FactoryGame")
{
	LOGF("Trying to load %s", *ClassName)
	UStruct* LoadedClass = LoadObject<UStruct>(nullptr, *ClassName);
	if (!LoadedClass->IsValidLowLevel())
	{
		LOGF("Trying to load /Script/%s.%s", *Package, *ClassName)
		LoadedClass = LoadObject<UStruct>(nullptr, *("/Script/" + Package + "." + ClassName));
		if (!LoadedClass->IsValidLowLevel())
		{
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
)
{
	LOG("FindDefaultComponentByName")
	FString GeneratedComponentName = ComponentName + "_GEN_VARIABLE";
	if (!IsValid(InActorClass))
	{
		LOG("Not valid")
		return nullptr;
	}
	// Check CDO.
	AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();
	TArray<UActorComponent*> FoundComponents;
	ActorCDO->GetComponents(InComponentClass, FoundComponents);

	for (auto Component : FoundComponents)
	{
		LOG(Component->GetName())
		if (Component->GetName() == ComponentName)
		{
			return Component;
		}
	}

	// Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from
	// CDO.
	UBlueprintGeneratedClass* RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);
	UClass* ActorClass = InActorClass;

	// Go down the inheritance tree to find nodes that were added to parent blueprints of our blueprint graph.
	do
	{
		LOG(ActorClass->GetName())
		UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
		if (!ActorBlueprintGeneratedClass)
		{
			LOG("not BlueprintGenerated")
			return nullptr;
		}

		const TArray<USCS_Node*>& ActorBlueprintNodes =
			ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

		for (USCS_Node* Node : ActorBlueprintNodes)
		{
			UClass* Class = Node->ComponentClass;
			LOG(Class->GetName())
			if (!Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->IsValidLowLevel()) { return nullptr; }
			if (Class->IsChildOf(InComponentClass) && Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->
			                                                GetName() == GeneratedComponentName)
			{
				return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);
			}
		}

		ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());
	}
	while (ActorClass != AActor::StaticClass());

	return nullptr;
}

//Yoinked from upcoming SML update, I needed it
UClass* FTIReflection::GenerateUniqueSimpleClass(const TCHAR* PackageName, const TCHAR* ClassName, UClass* ParentClass)
{
	LOGF("Creating subclass for %s", *ParentClass->GetFullName())
	LOGF("%ls", *(FString(PackageName) + "." + ClassName))
	UClass* LoadedClass = LoadObject<UClass>(nullptr, *(FString(PackageName) + "." + ClassName));
	LOGF("%hhd", LoadedClass->IsValidLowLevel())
	if (LoadedClass->IsValidLowLevel())
	{
		LOG("A class with this path already exists")
		return LoadedClass;
	}
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
	ConstructedClassObject = static_cast<UClass*>(GUObjectAllocator.AllocateUObject(
		sizeof(UClass), alignof(UClass), true));
	::new(ConstructedClassObject)UClass(
		EC_StaticConstructor,
		ClassName,
		ParentClass->GetStructureSize(),
		ParentClass->GetMinAlignment(),
		CLASS_Intrinsic,
		CASTCLASS_None,
		UObject::StaticConfigName(),
		static_cast<EObjectFlags>(RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet),
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
	if (!ConstructedClassObject->GetClass())
	{
		LOG("Registering")
		ConstructedClassObject->DeferredRegister(UClass::StaticClass(), PackageName, ClassName);
	}
	LOG("Out")
	//Mark class as Constructed and perform linking
	ConstructedClassObject->ClassFlags |= ParamsClassFlags | CLASS_Constructed;
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
	for (UProperty* Property = ConstructedClassObject->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		Property->CopyCompleteValue_InContainer(CDO, ParentCDO);
	}

	if (UBlueprintGeneratedClass* ClassAsBPGC = Cast<UBlueprintGeneratedClass>(ConstructedClassObject))
	{
		LOG("Dumping Construction Script")
		for (USCS_Node* Node : ClassAsBPGC->SimpleConstructionScript->GetAllNodes())
		{
			LOG(Node->ComponentClass->GetName())
		}
	}

	LOG(ConstructedClassObject->GetFullName())
	return ConstructedClassObject;
}

void* FTIReflection::MakeStructInstance(UStruct* Struct)
{
	void* instance = FMemory::Malloc(Struct->GetStructureSize());
	Struct->InitializeStruct(instance);
	return instance;
}

void* FTIReflection::CopyStruct(UStruct* Struct, void* Values)
{
	void* Copy = MakeStructInstance(Struct);
	for (FProperty* Property = Struct->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		Property->CopyCompleteValue_InContainer(Copy, Values);
	}
	return Copy;
}

FPropertyParamsBase MakeGenericParams(FProperty* Prop, EPropertyGenFlags Type)
{
	FGenericPropertyParams Generic = FGenericPropertyParams{
		TCHAR_TO_UTF8(*Prop->GetName()),
		TCHAR_TO_UTF8(*Prop->RepNotifyFunc.ToString()),
		Prop->PropertyFlags, Type,
		Prop->GetFlags(),
		Prop->ArrayDim,
		0};
	return *reinterpret_cast<FPropertyParamsBase*>(&Generic);
}

FProperty* FTIReflection::CopyProperty(FFieldVariant Outer, FProperty* Prop)
{
	if (FByteProperty* TypedProp = CastField<FByteProperty>(Prop))
	{
		// TODO: Support Enum
		FBytePropertyParams Params = {
			TCHAR_TO_UTF8(*Prop->GetName()),
			TCHAR_TO_UTF8(*Prop->RepNotifyFunc.ToString()),
			Prop->PropertyFlags,
			EPropertyGenFlags::Byte,
			Prop->GetFlags(),
			Prop->ArrayDim,
			0,
			nullptr,
			};
		NewProp = new FByteProperty(Outer, *Prop->GetName(), Prop->GetFlags(), Prop->Offset, Prop->PropertyFlags, Prop->EnumFunc ? Prop->EnumFunc() : nullptr);
	}

	if (FInt8Property* TypedProp = CastField<FInt8Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Int8);
	}

	if (FInt16Property* TypedProp = CastField<FInt16Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Int16);
	}

	if (FIntProperty* TypedProp = CastField<FIntProperty>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Int);
	}

	if (FInt64Property* TypedProp = CastField<FInt64Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Int64);
	}

	if (FUInt16Property* TypedProp = CastField<FUInt16Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::UInt16);
	}

	if (FUInt32Property* TypedProp = CastField<FUInt32Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::UInt32);
	}

	if (FUInt64Property* TypedProp = CastField<FUInt64Property>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::UInt64);
	}

	if (FFloatProperty* TypedProp = CastField<FFloatProperty>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Float);
	}

	if (FDoubleProperty* TypedProp = CastField<FDoubleProperty>(Prop))
	{
		return MakeGenericParams(Prop, EPropertyGenFlags::Double);
	}

	if (FBoolProperty* TypedProp = CastField<FBoolProperty>(Prop))
	{
		FString OuterSize;
		FString Setter;
		if (!Prop->GetOwner<UObject>())
		{
			OuterSize = TEXT("0");
			Setter    = TEXT("nullptr");
		}
		else
		{
			OuterSize = FString::Printf(TEXT("sizeof(%s)"), SourceStruct);

			DeclOut.Logf(TEXT("%sstatic void %s_SetBit(void* Obj);\r\n"), DeclSpaces, *NameWithoutScope);

			Out.Logf(TEXT("%svoid %s_SetBit(void* Obj)\r\n"), Spaces, Name);
			Out.Logf(TEXT("%s{\r\n"), Spaces);
			Out.Logf(TEXT("%s\t((%s*)Obj)->%s%s = 1;\r\n"), Spaces, SourceStruct, *Prop->GetName(), Prop->HasAllPropertyFlags(CPF_Deprecated) ? TEXT("_DEPRECATED") : TEXT(""));
			Out.Logf(TEXT("%s}\r\n"), Spaces);

			Setter = FString::Printf(TEXT("&%s_SetBit"), Name);
		}

		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FBoolPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FBoolPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Bool %s, %s, %s, sizeof(%s), %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			TypedProp->IsNativeBool() ? TEXT("| UE4CodeGen_Private::EPropertyGenFlags::NativeBool") : TEXT(""),
			FPropertyObjectFlags,
			*ArrayDim,
			*TypedProp->GetCPPType(nullptr, 0),
			*OuterSize,
			*Setter,
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FSoftClassProperty* TypedProp = CastField<FSoftClassProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FSoftClassPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FSoftClassPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::SoftClass, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->MetaClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FWeakObjectProperty* TypedProp = CastField<FWeakObjectProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FWeakObjectPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FWeakObjectPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::WeakObject, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->PropertyClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FLazyObjectProperty* TypedProp = CastField<FLazyObjectProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FLazyObjectPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FLazyObjectPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::LazyObject, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->PropertyClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FSoftObjectProperty* TypedProp = CastField<FSoftObjectProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FSoftObjectPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FSoftObjectPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::SoftObject, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->PropertyClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FClassProperty* TypedProp = CastField<FClassProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FClassPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FClassPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Class, %s, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->MetaClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*GetSingletonNameFuncAddr(TypedProp->PropertyClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FObjectProperty* TypedProp = CastField<FObjectProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FObjectPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FObjectPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Object, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->PropertyClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FInterfaceProperty* TypedProp = CastField<FInterfaceProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FInterfacePropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FInterfacePropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Interface, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->InterfaceClass, OutReferenceGatherers.UniqueCrossModuleReferences, false),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FNameProperty* TypedProp = CastField<FNameProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FNamePropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FNamePropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Name, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FStrProperty* TypedProp = CastField<FStrProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FStrPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FStrPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Str, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FArrayProperty* TypedProp = CastField<FArrayProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FArrayPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FArrayPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Array, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			GPropertyUsesMemoryImageAllocator.Contains(TypedProp) ? TEXT("EArrayPropertyFlags::UsesMemoryImageAllocator") : TEXT("EArrayPropertyFlags::None"),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FMapProperty* TypedProp = CastField<FMapProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FMapPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FMapPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Map, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			GPropertyUsesMemoryImageAllocator.Contains(TypedProp) ? TEXT("EMapPropertyFlags::UsesMemoryImageAllocator") : TEXT("EMapPropertyFlags::None"),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FSetProperty* TypedProp = CastField<FSetProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FSetPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FSetPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Set, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FStructProperty* TypedProp = CastField<FStructProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FStructPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FStructPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Struct, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->Struct, OutReferenceGatherers.UniqueCrossModuleReferences),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FDelegateProperty* TypedProp = CastField<FDelegateProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FDelegatePropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FDelegatePropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Delegate, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->SignatureFunction, OutReferenceGatherers.UniqueCrossModuleReferences),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FMulticastDelegateProperty* TypedProp = CastField<FMulticastDelegateProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FMulticastDelegatePropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FMulticastDelegatePropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::%sMulticastDelegate, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			(TypedProp->IsA<FMulticastInlineDelegateProperty>() ? TEXT("Inline") : TEXT("Sparse")),
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->SignatureFunction, OutReferenceGatherers.UniqueCrossModuleReferences),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FTextProperty* TypedProp = CastField<FTextProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FTextPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FTextPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Text, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FEnumProperty* TypedProp = CastField<FEnumProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FEnumPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FEnumPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::Enum, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*GetSingletonNameFuncAddr(TypedProp->Enum, OutReferenceGatherers.UniqueCrossModuleReferences),
			*MetaDataParams,
			*PropTag
		);

		return;
	}

	if (FFieldPathProperty* TypedProp = CastField<FFieldPathProperty>(Prop))
	{
		DeclOut.Logf(TEXT("%sstatic const UE4CodeGen_Private::FFieldPathPropertyParams %s;\r\n"), DeclSpaces, *NameWithoutScope);

		Out.Logf(
			TEXT("%sconst UE4CodeGen_Private::FFieldPathPropertyParams %s = { %s, %s, (EPropertyFlags)0x%016llx, UE4CodeGen_Private::EPropertyGenFlags::FieldPath, %s, %s, %s, %s, %s };%s\r\n"),
			Spaces,
			Name,
			*PropName,
			*PropNotifyFunc,
			PropFlags,
			FPropertyObjectFlags,
			*ArrayDim,
			OffsetStr,
			*FString::Printf(TEXT("&F%s::StaticClass"), *TypedProp->PropertyClass->GetName()),
			*MetaDataParams,
			*PropTag
		);

		return;
	}
}
