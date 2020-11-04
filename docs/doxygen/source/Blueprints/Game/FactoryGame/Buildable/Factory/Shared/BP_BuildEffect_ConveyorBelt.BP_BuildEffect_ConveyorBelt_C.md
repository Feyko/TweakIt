\page BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBelt BP_BuildEffect_ConveyorBelt
### Parent Class :
Actor### Asset Path :
<b><blockquote>/Game/FactoryGame/Buildable/Factory/-Shared/BP_BuildEffect_ConveyorBelt.BP_BuildEffect_ConveyorBelt_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBeltDefaultSceneRoot

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBeltSpline

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBeltMaterializeParticle

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBeltAk_StartLoop

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_ConveyorBeltAk_StopLoop

## Properties

### MoveParticleEffect_Time_4811E78B4C611AB8FA5FFAA13FD3D646
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>float</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>float</blockquote>
</details>
@endhtmlonly

### MoveParticleEffect__Direction_4811E78B4C611AB8FA5FFAA13FD3D646
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>byte</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>byte</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_e_timeline_direction.html"><blockquote>ETimelineDirection</blockquote></a></b>
</details>
@endhtmlonly

### mMesh
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_static_mesh.html"><blockquote>StaticMesh</blockquote></a></b>
</details>
@endhtmlonly

### mMaterialBelt
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable-shared_material_conveyor_belt__materialize__inst.html"><blockquote>ConveyorBelt_Materialize_Inst</blockquote></a></b>
</details>
@endhtmlonly

### mMaterialSides
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable-shared_material_conveyor_factory__materialize__inst.html"><blockquote>ConveyorFactory_Materialize_Inst</blockquote></a></b>
</details>
@endhtmlonly

### mSplinePointLocationArray
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>struct</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>struct</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_vector.html"><blockquote>Vector</blockquote></a></b>
</details>
<details>
 <summary>ContainerType</summary>
<blockquote>1</blockquote>
</details>
@endhtmlonly

### mSplinePointTangentArray
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>struct</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>struct</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_vector.html"><blockquote>Vector</blockquote></a></b>
</details>
<details>
 <summary>ContainerType</summary>
<blockquote>1</blockquote>
</details>
@endhtmlonly

### mSplineMeshLength
@htmlonly
<blockquote>200</blockquote>
@endhtmlonly

### Local_CurrentSplineMesh
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_spline_mesh_component.html"><blockquote>SplineMeshComponent</blockquote></a></b>
</details>
@endhtmlonly

### mDelayDuration
@htmlonly
<blockquote>0.10000000149011612</blockquote>
@endhtmlonly

### mLoopIndex
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>int</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>int</blockquote>
</details>
@endhtmlonly

### mSegmentList
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_blueprints_game_factory_game_buildable_factory-shared_b_p__build_effect__conveyor_belt_segment.html"><blockquote>BP_BuildEffect_ConveyorBeltSegment</blockquote></a></b>
</details>
<details>
 <summary>ContainerType</summary>
<blockquote>1</blockquote>
</details>
@endhtmlonly

### Particle
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>Object</blockquote>
</details>
<details>
 <summary>PinSubCategoryObject</summary>
<b><a href="_class_script_particle_system_component.html"><blockquote>ParticleSystemComponent</blockquote></a></b>
</details>
@endhtmlonly

### mSplineInitialized
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>bool</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>bool</blockquote>
</details>
@endhtmlonly

### mWantToStartEffect
@htmlonly
<details>
 <summary>PinCategory</summary>
<blockquote>bool</blockquote>
</details>
<details>
 <summary>PinSubCategory</summary>
<blockquote>bool</blockquote>
</details>
@endhtmlonly

### PrimaryActorTick
@htmlonly
<details>
 <summary>TickGroup</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>EndTickGroup</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>bTickEvenWhenPaused</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bCanEverTick</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bStartWithTickEnabled</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAllowTickOnDedicatedServer</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>TickInterval</summary>
<blockquote>0</blockquote>
</details>
@endhtmlonly

