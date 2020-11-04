\page BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyController BP_EnemyController
### Parent Class :
[FGEnemyController](@ref ClassScriptFGEnemyController)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Character/Creature/Enemy/BP_EnemyController.BP_EnemyController_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyControllerPathFollowingComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyControllerPerceptionComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyControllerActionsComp

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyControllerTransformComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyControllerRootComponent

## Properties

### mBlackboardAsset
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_b_b__enemy_blackboard.html"><blockquote>BB_EnemyBlackboard</blockquote></a></b>
</details>
@endhtmlonly

### mBehaviorTree
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_walker_b_t__walker.html"><blockquote>BT_Walker</blockquote></a></b>
</details>
@endhtmlonly

### mLastKnownTargetLocationBBKeyName
@htmlonly
<blockquote>LastKnownTargetLocation</blockquote>
@endhtmlonly

### mUpdateAggroInterval
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mTimeToLoseAllAggro
@htmlonly
<blockquote>10</blockquote>
@endhtmlonly

### mAggroBaseWeight
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mAggroAggroWeight
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mAggroDistanceWeight
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mTargetSwitchFactor
@htmlonly
<blockquote>0.30000001192092896</blockquote>
@endhtmlonly

### mDefaultIgnoreCooldown
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mStaticIgnoreCooldown
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mAggroDistanceCurve
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_a_i_distance_aggro_curve.html"><blockquote>DistanceAggroCurve</blockquote></a></b>
</details>
@endhtmlonly

### mLoseAggroThreshold
@htmlonly
<blockquote>-1</blockquote>
@endhtmlonly

### mIgnoredAggroTargetClasses
@htmlonly
<ol>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_train_wagon_b_p__freight_wagon.html"><blockquote>BP_FreightWagon</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_train_locomotive_b_p__locomotive.html"><blockquote>BP_Locomotive</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_train_locomotive_b_p__locomotive.html"><blockquote>BP_Locomotive</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle-shared_passenger_seat_b_p__passenger_seat.html"><blockquote>BP_PassengerSeat</blockquote></a></b>
</li>
<li>
<b><a href="_class_script_f_g_freight_wagon.html"><blockquote>FGFreightWagon</blockquote></a></b>
</li>
<li>
<b><a href="_class_script_f_g_locomotive.html"><blockquote>FGLocomotive</blockquote></a></b>
</li>
<li>
<b><a href="_class_script_f_g_passenger_seat.html"><blockquote>FGPassengerSeat</blockquote></a></b>
</li>
<li>
<b><a href="_class_script_f_g_railroad_vehicle.html"><blockquote>FGRailroadVehicle</blockquote></a></b>
</li>
</ol>
@endhtmlonly

### mPanicIgnoreTime
@htmlonly
<blockquote>3</blockquote>
@endhtmlonly

### PathFollowingComponent
@htmlonly
@endhtmlonly

### PerceptionComponent
@htmlonly
<details>
 <summary>SensesConfig</summary>
<ol>
<li>
<details>
 <summary>$ObjectClass</summary>
<b><a href="_class_script_a_i_sense_config__sight.html"><blockquote>AISenseConfig_Sight</blockquote></a></b>
</details>
<details>
 <summary>$ObjectFlags</summary>
<blockquote>2621481</blockquote>
</details>
<details>
 <summary>$ObjectName</summary>
<blockquote>AISenseConfig_Sight_0</blockquote>
</details>
<details>
 <summary>SightRadius</summary>
<blockquote>4500</blockquote>
</details>
<details>
 <summary>LoseSightRadius</summary>
<blockquote>5000</blockquote>
</details>
<details>
 <summary>PeripheralVisionAngleDegrees</summary>
<blockquote>179</blockquote>
</details>
<details>
 <summary>bStartsEnabled</summary>
<blockquote>False</blockquote>
</details>
</li>
</ol>
</details>
<details>
 <summary>DominantSense</summary>
<b><a href="_class_script_a_i_sense__sight.html"><blockquote>AISense_Sight</blockquote></a></b>
</details>
<details>
 <summary>OnTargetPerceptionUpdated</summary>
<blockquote>0</blockquote>
</details>
@endhtmlonly

### ActionsComp
@htmlonly
@endhtmlonly

### TransformComponent
@htmlonly
<details>
 <summary>bAbsoluteRotation</summary>
<blockquote>True</blockquote>
</details>
@endhtmlonly

### RootComponent
@htmlonly
<details>
 <summary>bAbsoluteRotation</summary>
<blockquote>True</blockquote>
</details>
@endhtmlonly

