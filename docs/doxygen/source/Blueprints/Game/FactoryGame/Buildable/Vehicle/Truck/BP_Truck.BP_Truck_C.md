\page BlueprintsGameFactoryGameBuildableVehicleTruckBP_Truck BP_Truck
### Parent Class :
[BP_WheeledVehicle6W](@ref BlueprintsGameFactoryGameBuildableVehicleBP_WheeledVehicle6W)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Buildable/Vehicle/Truck/BP_Truck.BP_Truck_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckSpringArm

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckCamera

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckPostProcess

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckBP_WorkBenchComponent

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckSpotLight

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckWorkbench

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckBP_WorkBenchComponent1

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckSpotLight1

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckmFoliageCollideBox

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckmMesh

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckmHealthComponent

@ref BlueprintsGameFactoryGameBuildableVehicleTruckBP_TruckRootComponent

## Properties

### mFlashlightOn
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

### mEngineSocketName
@htmlonly
<blockquote>Truck_AudioSocket_Engine</blockquote>
@endhtmlonly

### mEngineLoopStart
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_audio_rework_play__truck__rework__engine.html"><blockquote>Play_Truck_Rework_Engine</blockquote></a></b>
</details>
@endhtmlonly

### mEngineLoopStop
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_audio_rework_stop__truck__rework__engine.html"><blockquote>Stop_Truck_Rework_Engine</blockquote></a></b>
</details>
@endhtmlonly

### mExhaustSocketName
@htmlonly
<blockquote>Truck_AudioSocket_Exhaust</blockquote>
@endhtmlonly

### mTireLoopStart
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_audio_rework_play__truck__rework__tires.html"><blockquote>Play_Truck_Rework_Tires</blockquote></a></b>
</details>
@endhtmlonly

### mTireLoopStop
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_audio_rework_stop__truck__rework__tires.html"><blockquote>Stop_Truck_Rework_Tires</blockquote></a></b>
</details>
@endhtmlonly

### mVehicleSpeedRTPC
@htmlonly
<blockquote>RTPC_Truck_Speed</blockquote>
@endhtmlonly

### mVehicleAccelerationStateRTPC
@htmlonly
<blockquote>RTPC_Truck_Load</blockquote>
@endhtmlonly

### mHonkSound
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_audio_play__vehicle__honks__truck.html"><blockquote>Play_Vehicle_Honks_Truck</blockquote></a></b>
</details>
@endhtmlonly

### mMapText
@htmlonly
<blockquote>NSLOCTEXT("", "C6E446454C28565CE598B6BE33DEF086", "Truck")</blockquote>
@endhtmlonly

### mIsUsingNewAudioGears
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mAudioGearRTPC
@htmlonly
<blockquote>RTPC_Truck_Gears</blockquote>
@endhtmlonly

### mRTPCOffloadState
@htmlonly
<blockquote>RTPC_Truck_OffloadState</blockquote>
@endhtmlonly

### MPlayVehicleTransmission
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_audio_rework_play__truck__rework__transmission.html"><blockquote>Play_Truck_Rework_Transmission</blockquote></a></b>
</details>
@endhtmlonly

### mFuelConsumption
@htmlonly
<blockquote>75</blockquote>
@endhtmlonly

### mDistBetweenDecals
@htmlonly
<blockquote>80</blockquote>
@endhtmlonly

### mDecalLifespan
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mTireTrackDecals
@htmlonly
<ol>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__cement.html"><blockquote>PhysMat_Cement</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__coral.html"><blockquote>PhysMat_Coral</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__grass.html"><blockquote>PhysMat_Grass</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__decal.html"><blockquote>Tiretrack_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__grass__high.html"><blockquote>PhysMat_Grass_High</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__decal.html"><blockquote>Tiretrack_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__gravel.html"><blockquote>PhysMat_Gravel</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__metal.html"><blockquote>PhysMat_Metal</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__moist.html"><blockquote>PhysMat_Moist</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__sand__decal.html"><blockquote>Tiretrack_Sand_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__rock.html"><blockquote>PhysMat_Rock</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__rock__gravel.html"><blockquote>PhysMat_Rock_Gravel</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__sand.html"><blockquote>PhysMat_Sand</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__sand__decal.html"><blockquote>Tiretrack_Sand_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__sand__cracked.html"><blockquote>PhysMat_Sand_Cracked</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__metal__decal.html"><blockquote>Tiretrack_Metal_Decal</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SurfacePhysicsMaterial</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game-shared_material_physical_material_phys_mat__soil.html"><blockquote>PhysMat_Soil</blockquote></a></b>
</details>
</details>
<details>
 <summary>DecalMaterialOverride</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_material_tiretrack__decal.html"><blockquote>Tiretrack_Decal</blockquote></a></b>
</details>
</details>
</li>
</ol>
@endhtmlonly

### mDecalSize
@htmlonly
<details>
 <summary>X</summary>
<blockquote>10</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>110</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>120</blockquote>
</details>
@endhtmlonly

### mFoliageDestroyRadius
@htmlonly
<blockquote>575</blockquote>
@endhtmlonly

### mAddedGroundAngularVelocityStrengthYaw
@htmlonly
<blockquote>45</blockquote>
@endhtmlonly

### mAddedGroundAngularVelocityStrengthPitch
@htmlonly
<blockquote>60</blockquote>
@endhtmlonly

### mAddedAirControlAngularVelocityStrengthYaw
@htmlonly
<blockquote>75</blockquote>
@endhtmlonly

### mAddedAirControlAngularVelocityStrengthPitch
@htmlonly
<blockquote>50</blockquote>
@endhtmlonly

### mNaturalAngularVelocityStrengthYaw
@htmlonly
<blockquote>25</blockquote>
@endhtmlonly

### mNaturalAngularVelocityStrengthPitch
@htmlonly
<blockquote>15</blockquote>
@endhtmlonly

### mNaturalAirAngularVelocityStrengthYaw
@htmlonly
<blockquote>20</blockquote>
@endhtmlonly

### mNaturalAirAngularVelocityStrengthPitch
@htmlonly
<blockquote>10</blockquote>
@endhtmlonly

### mAddedAngularVelocityInputSmoothingSpeed
@htmlonly
<blockquote>0.10000000149011612</blockquote>
@endhtmlonly

### mFoliageCollideBox
@htmlonly
<details>
 <summary>BoxExtent</summary>
<details>
 <summary>X</summary>
<blockquote>560</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>370</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>320</blockquote>
</details>
</details>
<details>
 <summary>AreaClass</summary>
<b><a href="_class_script_nav_area__obstacle.html"><blockquote>NavArea_Obstacle</blockquote></a></b>
</details>
<details>
 <summary>BodyInstance</summary>
<details>
 <summary>ObjectType</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>CollisionEnabled</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>SleepFamily</summary>
<blockquote>ESleepFamily::Normal</blockquote>
</details>
<details>
 <summary>DOFMode</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>bUseCCD</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bNotifyRigidBodyCollision</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bSimulatePhysics</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMass</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bEnableGravity</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAutoWeld</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bStartAwake</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bGenerateWakeEvents</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bUpdateMassWhenScaleChanges</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockTranslation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockRotation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockXTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockXRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxAngularVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxDepenetrationVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideWalkableSlopeOnInstance</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bInterpolateWhenSubStepping</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>CollisionProfileName</summary>
<blockquote>Custom</blockquote>
</details>
<details>
 <summary>CollisionResponses</summary>
<details>
 <summary>ResponseArray</summary>
<ol>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WorldStatic</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WorldDynamic</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Pawn</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Visibility</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Camera</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>PhysicsBody</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Vehicle</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Destructible</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>Projectile</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>BuildGun</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WeaponInstantHit</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>VehicleWheelQuery</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
</ol>
</details>
</details>
<details>
 <summary>MaxDepenetrationVelocity</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>MassInKgOverride</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>LinearDamping</summary>
<blockquote>0.009999999776482582</blockquote>
</details>
<details>
 <summary>AngularDamping</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>CustomDOFPlaneNormal</summary>
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>COMNudge</summary>
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>MassScale</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>InertiaTensorScale</summary>
<details>
 <summary>X</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>1</blockquote>
</details>
</details>
<details>
 <summary>WalkableSlopeOverride</summary>
<details>
 <summary>WalkableSlopeBehavior</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>WalkableSlopeAngle</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>PhysMaterialOverride</summary>
<details>
 <summary>$Empty</summary>
<blockquote>True</blockquote>
</details>
</details>
<details>
 <summary>MaxAngularVelocity</summary>
<blockquote>3600</blockquote>
</details>
<details>
 <summary>CustomSleepThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>StabilizationThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>PhysicsBlendWeight</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>PositionSolverIterationCount</summary>
<blockquote>8</blockquote>
</details>
<details>
 <summary>VelocitySolverIterationCount</summary>
<blockquote>1</blockquote>
</details>
</details>
<details>
 <summary>AttachParent</summary>
<details>
 <summary>$ObjectClass</summary>
<b><a href="_class_script_skeletal_mesh_component.html"><blockquote>SkeletalMeshComponent</blockquote></a></b>
</details>
<details>
 <summary>$ObjectFlags</summary>
<blockquote>2883617</blockquote>
</details>
<details>
 <summary>$ObjectName</summary>
<blockquote>VehicleMesh</blockquote>
</details>
<details>
 <summary>AnimClass</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_anim__truck.html"><blockquote>Anim_Truck</blockquote></a></b>
</details>
<details>
 <summary>SkeletalMesh</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_mesh_truck.html"><blockquote>Truck</blockquote></a></b>
</details>
</details>
<details>
 <summary>bGenerateOverlapEvents</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAllowCullDistanceVolume</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>BodyInstance</summary>
<details>
 <summary>ObjectType</summary>
<blockquote>6</blockquote>
</details>
<details>
 <summary>CollisionEnabled</summary>
<blockquote>3</blockquote>
</details>
<details>
 <summary>SleepFamily</summary>
<blockquote>ESleepFamily::Normal</blockquote>
</details>
<details>
 <summary>DOFMode</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>bUseCCD</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bNotifyRigidBodyCollision</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bSimulatePhysics</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bOverrideMass</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bEnableGravity</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAutoWeld</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bStartAwake</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bGenerateWakeEvents</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bUpdateMassWhenScaleChanges</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockTranslation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockRotation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockXTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockXRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxAngularVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxDepenetrationVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideWalkableSlopeOnInstance</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bInterpolateWhenSubStepping</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>CollisionProfileName</summary>
<blockquote>Vehicle</blockquote>
</details>
<details>
 <summary>CollisionResponses</summary>
<details>
 <summary>ResponseArray</summary>
<ol>
<li>
<details>
 <summary>Channel</summary>
<blockquote>BuildGun</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WeaponInstantHit</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>HologramClearance</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
</ol>
</details>
</details>
<details>
 <summary>MaxDepenetrationVelocity</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>MassInKgOverride</summary>
<blockquote>2401</blockquote>
</details>
<details>
 <summary>LinearDamping</summary>
<blockquote>0.009999999776482582</blockquote>
</details>
<details>
 <summary>AngularDamping</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>CustomDOFPlaneNormal</summary>
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>COMNudge</summary>
<details>
 <summary>X</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>MassScale</summary>
<blockquote>10</blockquote>
</details>
<details>
 <summary>InertiaTensorScale</summary>
<details>
 <summary>X</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>1</blockquote>
</details>
</details>
<details>
 <summary>WalkableSlopeOverride</summary>
<details>
 <summary>WalkableSlopeBehavior</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>WalkableSlopeAngle</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>PhysMaterialOverride</summary>
<details>
 <summary>$Empty</summary>
<blockquote>True</blockquote>
</details>
</details>
<details>
 <summary>MaxAngularVelocity</summary>
<blockquote>3600</blockquote>
</details>
<details>
 <summary>CustomSleepThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>StabilizationThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>PhysicsBlendWeight</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>PositionSolverIterationCount</summary>
<blockquote>8</blockquote>
</details>
<details>
 <summary>VelocitySolverIterationCount</summary>
<blockquote>1</blockquote>
</details>
</details>
</details>
<details>
 <summary>RelativeLocation</summary>
<details>
 <summary>X</summary>
<blockquote>-90</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>280</blockquote>
</details>
</details>
@endhtmlonly

### mInventorySize
@htmlonly
<blockquote>48</blockquote>
@endhtmlonly

### mIsPathVisible
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mReverseAddedAngularVelocityYawMultiplier
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mHasAirControl
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mGroundTraceLength
@htmlonly
<blockquote>300</blockquote>
@endhtmlonly

### mMaxDeltaLinearVelocity
@htmlonly
<blockquote>100</blockquote>
@endhtmlonly

### mMaxDeltaAngularVelocity
@htmlonly
<blockquote>550</blockquote>
@endhtmlonly

### mRollStabilisationStrength
@htmlonly
<blockquote>0.026000000536441803</blockquote>
@endhtmlonly

### mMaxRollAngleForUpsideDown
@htmlonly
<blockquote>85</blockquote>
@endhtmlonly

### mMaxFlatOnGroundRollAngleLimit
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mMaxRollForActivationOfAssistedVelocities
@htmlonly
<blockquote>75</blockquote>
@endhtmlonly

### mMaxSpeedForAddedAcceleration
@htmlonly
<blockquote>80</blockquote>
@endhtmlonly

### mMaxAssistedAcceleration
@htmlonly
<blockquote>500</blockquote>
@endhtmlonly

### mDriftingLateralForce
@htmlonly
<blockquote>2057680</blockquote>
@endhtmlonly

### mDriftingUpwardForce
@htmlonly
<blockquote>800000000</blockquote>
@endhtmlonly

### mDriftForwardForceStrengthCurve
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_truck_drift_forward_force_curve.html"><blockquote>TruckDriftForwardForceCurve</blockquote></a></b>
</details>
@endhtmlonly

### mDriftForceBones
@htmlonly
<ol>
<li>
<blockquote>drift_force_r</blockquote>
</li>
<li>
<blockquote>drift_force_l</blockquote>
</li>
<li>
<blockquote>Root</blockquote>
</li>
</ol>
@endhtmlonly

### mMinAngleForDrift
@htmlonly
<blockquote>7</blockquote>
@endhtmlonly

### mNeedsFuelToDrive
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mDisplayName
@htmlonly
<blockquote>NSLOCTEXT("", "4E99961D424677421EF8F19BF61C314F", "Truck")</blockquote>
@endhtmlonly

### mDescription
@htmlonly
<blockquote>NSLOCTEXT("", "EF0DEB594BDC94DC4CD59097330BD008", "Fuel: Any fuel type\r\n\r\n48 slot inventory. Has a built in craft bench.\r\nCan be automated to pick up and deliver resources at truck stations.\r\n\r\nNicknamed \'the Unit\' by FICSIT pioneers because of its massive frame.")</blockquote>
@endhtmlonly

### mHologramClass
@htmlonly
<b><a href="_class_script_f_g_vehicle_hologram.html"><blockquote>FGVehicleHologram</blockquote></a></b>
@endhtmlonly

### mMesh
@htmlonly
<details>
 <summary>AnimClass</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_anim__truck.html"><blockquote>Anim_Truck</blockquote></a></b>
</details>
<details>
 <summary>SkeletalMesh</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_mesh_truck.html"><blockquote>Truck</blockquote></a></b>
</details>
</details>
<details>
 <summary>bGenerateOverlapEvents</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAllowCullDistanceVolume</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>BodyInstance</summary>
<details>
 <summary>ObjectType</summary>
<blockquote>6</blockquote>
</details>
<details>
 <summary>CollisionEnabled</summary>
<blockquote>3</blockquote>
</details>
<details>
 <summary>SleepFamily</summary>
<blockquote>ESleepFamily::Normal</blockquote>
</details>
<details>
 <summary>DOFMode</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>bUseCCD</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bNotifyRigidBodyCollision</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bSimulatePhysics</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bOverrideMass</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bEnableGravity</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAutoWeld</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bStartAwake</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bGenerateWakeEvents</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bUpdateMassWhenScaleChanges</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockTranslation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockRotation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockXTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockXRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxAngularVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxDepenetrationVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideWalkableSlopeOnInstance</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bInterpolateWhenSubStepping</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>CollisionProfileName</summary>
<blockquote>Vehicle</blockquote>
</details>
<details>
 <summary>CollisionResponses</summary>
<details>
 <summary>ResponseArray</summary>
<ol>
<li>
<details>
 <summary>Channel</summary>
<blockquote>BuildGun</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WeaponInstantHit</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>HologramClearance</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
</ol>
</details>
</details>
<details>
 <summary>MaxDepenetrationVelocity</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>MassInKgOverride</summary>
<blockquote>2401</blockquote>
</details>
<details>
 <summary>LinearDamping</summary>
<blockquote>0.009999999776482582</blockquote>
</details>
<details>
 <summary>AngularDamping</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>CustomDOFPlaneNormal</summary>
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>COMNudge</summary>
<details>
 <summary>X</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>MassScale</summary>
<blockquote>10</blockquote>
</details>
<details>
 <summary>InertiaTensorScale</summary>
<details>
 <summary>X</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>1</blockquote>
</details>
</details>
<details>
 <summary>WalkableSlopeOverride</summary>
<details>
 <summary>WalkableSlopeBehavior</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>WalkableSlopeAngle</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>PhysMaterialOverride</summary>
<details>
 <summary>$Empty</summary>
<blockquote>True</blockquote>
</details>
</details>
<details>
 <summary>MaxAngularVelocity</summary>
<blockquote>3600</blockquote>
</details>
<details>
 <summary>CustomSleepThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>StabilizationThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>PhysicsBlendWeight</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>PositionSolverIterationCount</summary>
<blockquote>8</blockquote>
</details>
<details>
 <summary>VelocitySolverIterationCount</summary>
<blockquote>1</blockquote>
</details>
</details>
@endhtmlonly

### mHealthComponent
@htmlonly
<details>
 <summary>mMaxHealth</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>mCurrentHealth</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>bNetAddressable</summary>
<blockquote>True</blockquote>
</details>
@endhtmlonly

### mDisabledByWaterLocations
@htmlonly
<ol>
<li>
<details>
 <summary>X</summary>
<blockquote>-30</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>250</blockquote>
</details>
</li>
</ol>
@endhtmlonly

### mPrimaryColor
@htmlonly
<details>
 <summary>R</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>G</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>B</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>A</summary>
<blockquote>1</blockquote>
</details>
@endhtmlonly

### mSecondaryColor
@htmlonly
<details>
 <summary>R</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>G</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>B</summary>
<blockquote>-1</blockquote>
</details>
<details>
 <summary>A</summary>
<blockquote>1</blockquote>
</details>
@endhtmlonly

### mSubmergedAngularDamping
@htmlonly
<blockquote>6</blockquote>
@endhtmlonly

### mSubmergedLinearDamping
@htmlonly
<blockquote>15</blockquote>
@endhtmlonly

### mSubmergedBouyantForce
@htmlonly
<blockquote>1000</blockquote>
@endhtmlonly

### mAddToSignificanceManager
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mSimulationDistance
@htmlonly
<blockquote>20000</blockquote>
@endhtmlonly

### mShouldAttachDriver
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### mDriverSeatSocket
@htmlonly
<blockquote>driverSocket</blockquote>
@endhtmlonly

### mDriverExitOffset
@htmlonly
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>500</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
@endhtmlonly

### RootComponent
@htmlonly
<details>
 <summary>AnimClass</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_anim__truck.html"><blockquote>Anim_Truck</blockquote></a></b>
</details>
<details>
 <summary>SkeletalMesh</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_mesh_truck.html"><blockquote>Truck</blockquote></a></b>
</details>
</details>
<details>
 <summary>bGenerateOverlapEvents</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAllowCullDistanceVolume</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>BodyInstance</summary>
<details>
 <summary>ObjectType</summary>
<blockquote>6</blockquote>
</details>
<details>
 <summary>CollisionEnabled</summary>
<blockquote>3</blockquote>
</details>
<details>
 <summary>SleepFamily</summary>
<blockquote>ESleepFamily::Normal</blockquote>
</details>
<details>
 <summary>DOFMode</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>bUseCCD</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bNotifyRigidBodyCollision</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bSimulatePhysics</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bOverrideMass</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bEnableGravity</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bAutoWeld</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bStartAwake</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bGenerateWakeEvents</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bUpdateMassWhenScaleChanges</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockTranslation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockRotation</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>bLockXTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZTranslation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockXRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockYRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bLockZRotation</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxAngularVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideMaxDepenetrationVelocity</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bOverrideWalkableSlopeOnInstance</summary>
<blockquote>False</blockquote>
</details>
<details>
 <summary>bInterpolateWhenSubStepping</summary>
<blockquote>True</blockquote>
</details>
<details>
 <summary>CollisionProfileName</summary>
<blockquote>Vehicle</blockquote>
</details>
<details>
 <summary>CollisionResponses</summary>
<details>
 <summary>ResponseArray</summary>
<ol>
<li>
<details>
 <summary>Channel</summary>
<blockquote>BuildGun</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>WeaponInstantHit</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>2</blockquote>
</details>
</li>
<li>
<details>
 <summary>Channel</summary>
<blockquote>HologramClearance</blockquote>
</details>
<details>
 <summary>Response</summary>
<blockquote>1</blockquote>
</details>
</li>
</ol>
</details>
</details>
<details>
 <summary>MaxDepenetrationVelocity</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>MassInKgOverride</summary>
<blockquote>2401</blockquote>
</details>
<details>
 <summary>LinearDamping</summary>
<blockquote>0.009999999776482582</blockquote>
</details>
<details>
 <summary>AngularDamping</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>CustomDOFPlaneNormal</summary>
<details>
 <summary>X</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>COMNudge</summary>
<details>
 <summary>X</summary>
<blockquote>100</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>MassScale</summary>
<blockquote>10</blockquote>
</details>
<details>
 <summary>InertiaTensorScale</summary>
<details>
 <summary>X</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Y</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>Z</summary>
<blockquote>1</blockquote>
</details>
</details>
<details>
 <summary>WalkableSlopeOverride</summary>
<details>
 <summary>WalkableSlopeBehavior</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>WalkableSlopeAngle</summary>
<blockquote>0</blockquote>
</details>
</details>
<details>
 <summary>PhysMaterialOverride</summary>
<details>
 <summary>$Empty</summary>
<blockquote>True</blockquote>
</details>
</details>
<details>
 <summary>MaxAngularVelocity</summary>
<blockquote>3600</blockquote>
</details>
<details>
 <summary>CustomSleepThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>StabilizationThresholdMultiplier</summary>
<blockquote>1</blockquote>
</details>
<details>
 <summary>PhysicsBlendWeight</summary>
<blockquote>0</blockquote>
</details>
<details>
 <summary>PositionSolverIterationCount</summary>
<blockquote>8</blockquote>
</details>
<details>
 <summary>VelocitySolverIterationCount</summary>
<blockquote>1</blockquote>
</details>
</details>
@endhtmlonly

