\page BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_Pipeline BP_BuildEffect_Pipeline
### Parent Class :
[FGBuildEffectSpline](@ref ClassScriptFGBuildEffectSpline)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Buildable/Factory/-Shared/BP_BuildEffect_Pipeline.BP_BuildEffect_Pipeline_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_PipelineDefaultSceneRoot

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_PipelineSpline

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_PipelineMaterializeParticle

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_PipelineAk_StartLoop

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_PipelineAk_StopLoop

## Properties

### MoveParticleEffect_Time_C5045958477A801600BF8BAD6657365D
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

### MoveParticleEffect__Direction_C5045958477A801600BF8BAD6657365D
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
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_factory_pipeline_mesh_pipeline.html"><blockquote>Pipeline</blockquote></a></b>
</details>
@endhtmlonly

### mPipeMaterial
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable-shared_material_pipeline__materialize__inst.html"><blockquote>Pipeline_Materialize_Inst</blockquote></a></b>
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

### mSegmentListan
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
<b><a href="_blueprints_game_factory_game_buildable_factory-shared_b_p__build_effect__pipeline_segment.html"><blockquote>BP_BuildEffect_PipelineSegment</blockquote></a></b>
</details>
<details>
 <summary>ContainerType</summary>
<blockquote>1</blockquote>
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

