#pragma once
#include "FGBuildable.h"
#include "Lua/Lua.h"

namespace Tweaker
{
    namespace Reflection
    {
        inline UActorComponent* FindDefaultComponentByName(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass, FString ComponentName)
        {
            FString GeneratedComponentName = ComponentName + "_GEN_VARIABLE";
            if (!IsValid(InActorClass))
            {
                return nullptr;
            }
            // Check CDO.
            AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();
            TArray<UActorComponent*> FoundComponents = ActorCDO->GetComponentsByClass(InComponentClass);

            for(auto Component : FoundComponents)
            {
                LOGFS(Component->GetName())
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
                UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
                if (!ActorBlueprintGeneratedClass)
                {
                    return nullptr;
                }
 
                const TArray<USCS_Node*>& ActorBlueprintNodes =
                    ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();
 
                for (USCS_Node* Node : ActorBlueprintNodes)
                {
                    UClass* Class = Node->ComponentClass;
                    if(!Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->IsValidLowLevel()) {return nullptr;}
                    if (Class->IsChildOf(InComponentClass) && Node->GetActualComponentTemplate(RootBlueprintGeneratedClass)->GetName() == GeneratedComponentName)
                    {
                        return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);
                    }
                }
 
                ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());
 
            } while (ActorClass != AActor::StaticClass());
 
            return nullptr;
        }

        inline UProperty* FindPropertyByShortName(UStruct* Class, const TCHAR* PropertyName) {
            for(UProperty* Property = Class->PropertyLink; Property; Property = Property->PropertyLinkNext) {
                if (Property->GetName().StartsWith(PropertyName)) {
                    return Property;
                }
            }
            return nullptr;
        }
    }
}
