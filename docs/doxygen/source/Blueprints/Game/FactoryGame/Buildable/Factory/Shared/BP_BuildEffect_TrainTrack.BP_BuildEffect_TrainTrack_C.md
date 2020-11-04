\page BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrack BP_BuildEffect_TrainTrack
### Parent Class :
Actor### Asset Path :
<b><blockquote>/Game/FactoryGame/Buildable/Factory/-Shared/BP_BuildEffect_TrainTrack.BP_BuildEffect_TrainTrack_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrackDefaultSceneRoot

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrackSpline

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrackMaterializeParticle

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrackAk_StartLoop

@ref BlueprintsGameFactoryGameBuildableFactorySharedBP_BuildEffect_TrainTrackAk_StopLoop

## Properties

### MoveParticleEffect_Time_82E420F745BD1A027C3D16B22990E87F
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

### MoveParticleEffect__Direction_82E420F745BD1A027C3D16B22990E87F
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
<b><a href="_blueprints_game_factory_game_buildable_factory_train_track_mesh_s_m__train_track_02.html"><blockquote>SM_TrainTrack_02</blockquote></a></b>
</details>
@endhtmlonly

### mTrainMaterial
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_factory_train_track_material_m_i__track__materialize.html"><blockquote>MI_Track_Materialize</blockquote></a></b>
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
<blockquote>1200</blockquote>
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
<b><a href="_blueprints_game_factory_game_buildable_factory-shared_b_p__build_effect__train_track_segment.html"><blockquote>BP_BuildEffect_TrainTrackSegment</blockquote></a></b>
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

