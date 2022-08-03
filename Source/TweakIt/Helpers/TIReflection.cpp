#include "TIReflection.h"

#include <functional>


#include "TIUFunctionBinder.h"
// #include "Editor/KismetCompiler/Public/KismetCompilerMisc.h"
#include "Engine/SCS_Node.h"
#include "TweakIt/Logging/FTILog.h"

UClass* FTIReflection::UFunctionOuterBuffer = nullptr;

FProperty* FTIReflection::FindPropertyByName(UStruct* Class, const TCHAR* PropertyName)
{
	FString lowered = FString(PropertyName).ToLower();
	for (FProperty* Property = Class->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		if (Property->GetName().ToLower() == lowered)
		{
			return Property;
		}
	}
	return nullptr;
}

UFunction* FTIReflection::FindFunctionByName(UStruct* Class, const TCHAR* PropertyName)
{
	FString lowered = FString(PropertyName).ToLower();
	for (auto Iterator = TFieldIterator<UFunction>(Class); Iterator; ++Iterator)
	{
		if (*Iterator->GetName().ToLower() == lowered)
		{
			return *Iterator;
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
	for (FProperty* Property = ConstructedClassObject->PropertyLink; Property; Property = Property->PropertyLinkNext)
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

UFunction* FTIReflection::CopyUFunction(UFunction* ToCopy, FString FunctionName, UClass* Outer /*= nullptr*/)
{
	UFunction* Function = nullptr;
	FFunctionParams Params = FFunctionParams();
	Params.OwningClassName = TCHAR_TO_UTF8(*UTIUFunctionBinder::StaticClass()->GetName());
	Params.NameUTF8 = TCHAR_TO_UTF8(*FunctionName);
	UFunctionOuterBuffer = Outer ? Outer : UTIUFunctionBinder::StaticClass();
	Params.OuterFunc = []()->UObject*{return UFunctionOuterBuffer;};
	Params.FunctionFlags = FUNC_Native|FUNC_Static|FUNC_Public;
	LOG("Constructing function")
	ConstructUFunction(Function, Params);
	LOG("Copying properties")
	for (auto Prop = ToCopy->PropertyLink; Prop; Prop = Prop->PropertyLinkNext)
	{
		CopyProperty(Function, Prop);
	}
	LOG("Reversing properties")
	// Properties are added to the front of the linked list.
	// We reverse it to make sure the properties are in the same order as they were in the input function
	ReverseChildProperties(&Function->ChildProperties);
	FArchiveUObject Dummy;
	LOG("Linking function")
	Function->Link(Dummy, true);
	return Function;
}

FProperty* FTIReflection::CopyProperty(FFieldVariant Outer, FProperty* Prop)
{
	if (FByteProperty* TypedProp = CastField<FByteProperty>(Prop))
	{
		//TODO Test Enum
		return new FByteProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->Enum);
	}

	if (FInt8Property* TypedProp = CastField<FInt8Property>(Prop))
	{
		return new FInt8Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FInt16Property* TypedProp = CastField<FInt16Property>(Prop))
	{
		return new FInt16Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FIntProperty* TypedProp = CastField<FIntProperty>(Prop))
	{
		return new FIntProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FInt64Property* TypedProp = CastField<FInt64Property>(Prop))
	{
		return new FInt64Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FUInt16Property* TypedProp = CastField<FUInt16Property>(Prop))
	{
		return new FUInt16Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FUInt32Property* TypedProp = CastField<FUInt32Property>(Prop))
	{
		return new FUInt32Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FUInt64Property* TypedProp = CastField<FUInt64Property>(Prop))
	{
		return new FUInt64Property(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FFloatProperty* TypedProp = CastField<FFloatProperty>(Prop))
	{
		return new FFloatProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FDoubleProperty* TypedProp = CastField<FDoubleProperty>(Prop))
	{
		return new FDoubleProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FBoolProperty* TypedProp = CastField<FBoolProperty>(Prop))
	{
		return new FBoolProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, GetBoolPropertyBitmask(TypedProp), Prop->ElementSize, Prop->IsNative());
	}

	if (FSoftClassProperty* TypedProp = CastField<FSoftClassProperty>(Prop))
	{
		return new FSoftClassProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->MetaClass);
	}

	if (FWeakObjectProperty* TypedProp = CastField<FWeakObjectProperty>(Prop))
	{
		return new FWeakObjectProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->PropertyClass);
	}

	if (FLazyObjectProperty* TypedProp = CastField<FLazyObjectProperty>(Prop))
	{
		return new FLazyObjectProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->PropertyClass);
	}

	if (FSoftObjectProperty* TypedProp = CastField<FSoftObjectProperty>(Prop))
	{
		return new FSoftObjectProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->PropertyClass);
	}

	if (FClassProperty* TypedProp = CastField<FClassProperty>(Prop))
	{
		//TODO Test
		return new FClassProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->MetaClass, TypedProp->PropertyClass);
	}

	if (FObjectProperty* TypedProp = CastField<FObjectProperty>(Prop))
	{
		return new FObjectProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->PropertyClass);
	}

	if (FInterfaceProperty* TypedProp = CastField<FInterfaceProperty>(Prop))
	{
		return new FInterfaceProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->InterfaceClass);
	}

	if (FNameProperty* TypedProp = CastField<FNameProperty>(Prop))
	{
		return new FNameProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FStrProperty* TypedProp = CastField<FStrProperty>(Prop))
	{
		return new FStrProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FArrayProperty* TypedProp = CastField<FArrayProperty>(Prop))
	{
		//TODO Inner property
		return new FArrayProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->ArrayFlags);
	}

	if (FMapProperty* TypedProp = CastField<FMapProperty>(Prop))
	{
		//TODO Inner properties
		return new FMapProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->MapFlags);
	}

	if (FSetProperty* TypedProp = CastField<FSetProperty>(Prop))
	{
		//TODO Inner property
		return new FSetProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FStructProperty* TypedProp = CastField<FStructProperty>(Prop))
	{
		//TODO Inner property?
		//TODO Test (inner struct)
		return new FStructProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->Struct);
	}

	if (FDelegateProperty* TypedProp = CastField<FDelegateProperty>(Prop))
	{
		return new FDelegateProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FMulticastDelegateProperty* TypedProp = CastField<FMulticastDelegateProperty>(Prop))
	{
		return new FMulticastDelegateProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FTextProperty* TypedProp = CastField<FTextProperty>(Prop))
	{
		return new FTextProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags);
	}

	if (FEnumProperty* TypedProp = CastField<FEnumProperty>(Prop))
	{
		return new FEnumProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->GetEnum());
	}

	if (FFieldPathProperty* TypedProp = CastField<FFieldPathProperty>(Prop))
	{
		return new FFieldPathProperty(Outer, *Prop->GetName(), Prop->GetFlags(), 0, Prop->PropertyFlags, TypedProp->PropertyClass);
	}
	return nullptr;
}

// TODO: Test
uint8 FTIReflection::GetBoolPropertyBitmask(FBoolProperty* Prop)
{
	TArray<uint8> Buf;
	Buf.Reserve(Prop->ElementSize);
	for (int i = 0; i < Prop->ElementSize; ++i)
	{
		Buf.Add(true);
	}
	Prop->SetPropertyValue(Buf.GetData(),true);
	for (auto Byte : Buf)
	{
		if (Byte > 0)
		{
			return Byte;
		}
	}
	return 0;
}

void FTIReflection::ReverseChildProperties(FField** Head)
{
	FField* Temp = nullptr;
	FField* Previous = nullptr;
	FField* Current = *Head;
	while (Current != nullptr)
	{
		Temp = Current->Next;
		Current->Next = Previous;
		Previous = Current;
		Current = Temp;
	}
	*Head = Previous;
}

template <typename T>
int FTIReflection::AddValueToEnum(FName NewName)
{
	UEnum* Enum = StaticEnum<T>();
	FString EnumName = Enum->GetName();
	if (!NewName.ToString().StartsWith(EnumName))
	{
		NewName = FName(FString::Printf(TEXT("%s::%s"), *EnumName, *NewName.ToString()));
	}
	if (Enum->IsValidEnumName(NewName))
	{
		return -1;
	}
	bool HasMax = Enum->ContainsExistingMax();
	int64 NewValue = HasMax ? Enum->GetMaxEnumValue() : Enum->NumEnums();
	TArray<TPair<FName, int64>> Names;
	int EnumNum = Enum->NumEnums();
	if (HasMax)
	{
		--EnumNum;
	}
	for (int i = 0; i < EnumNum; ++i)
	{
		FName Name = Enum->GetNameByIndex(i);
		int64 Value = Enum->GetValueByIndex(i);
		Names.Emplace(TPair<FName, int64>(Name, Value));
	}
	auto New = TPair<FName, int64>(NewName, NewValue);
	Names.Emplace(New);
	EEnumFlags Flag = Enum->HasAnyEnumFlags(EEnumFlags::Flags) ? EEnumFlags::Flags : EEnumFlags::None;
	Enum->SetEnums(Names, Enum->GetCppForm(), Flag, HasMax);
	return EnumNum;
}