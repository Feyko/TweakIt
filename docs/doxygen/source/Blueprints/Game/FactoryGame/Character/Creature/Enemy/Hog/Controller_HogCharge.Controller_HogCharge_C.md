\page BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogCharge Controller_HogCharge
### Parent Class :
[BP_EnemyController](@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyController)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Character/Creature/Enemy/Hog/Controller_HogCharge.Controller_HogCharge_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogChargePathFollowingComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogChargePerceptionComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogChargeActionsComp

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogChargeTransformComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyHogController_HogChargeRootComponent

## Properties

### mIsChargingAtEnemy
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

### mFinalGoalKeyName
@htmlonly
<blockquote>FinalGoal</blockquote>
@endhtmlonly

### mHearNoiseKeyName
@htmlonly
<blockquote>DidHearNoise</blockquote>
@endhtmlonly

### mNoiseLocationKeyName
@htmlonly
<blockquote>NoiseLocation</blockquote>
@endhtmlonly

### mDoPanicName
@htmlonly
<blockquote>DoPanic</blockquote>
@endhtmlonly

### mBlackboardAsset
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_hog_a_i_b_b__hog.html"><blockquote>BB_Hog</blockquote></a></b>
</details>
@endhtmlonly

### mBehaviorTree
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_hog_a_i_b_t__hog.html"><blockquote>BT_Hog</blockquote></a></b>
</details>
@endhtmlonly

### mUpdateAggroInterval
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mTimeToLoseAllAggro
@htmlonly
<blockquote>6</blockquote>
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
<blockquote>1.100000023841858</blockquote>
@endhtmlonly

### mDefaultIgnoreCooldown
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

### mStaticIgnoreCooldown
@htmlonly
<blockquote>30</blockquote>
@endhtmlonly

### mAggroDistanceCurve
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_hog_curve__distance_aggro_hog.html"><blockquote>Curve_DistanceAggroHog</blockquote></a></b>
</details>
@endhtmlonly

### mGainAggroThreshold
@htmlonly
<blockquote>0.800000011920929</blockquote>
@endhtmlonly

### mLoseAggroThreshold
@htmlonly
<blockquote>0.6899999976158142</blockquote>
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
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_tractor_b_p__tractor.html"><blockquote>BP_Tractor</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_truck_b_p__truck.html"><blockquote>BP_Truck</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_buildable_vehicle_explorer_b_p__explorer.html"><blockquote>BP_Explorer</blockquote></a></b>
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
<blockquote>2621473</blockquote>
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
<li>
<details>
 <summary>$ObjectClass</summary>
<b><a href="_class_script_a_i_sense_config__hearing.html"><blockquote>AISenseConfig_Hearing</blockquote></a></b>
</details>
<details>
 <summary>$ObjectFlags</summary>
<blockquote>2621481</blockquote>
</details>
<details>
 <summary>$ObjectName</summary>
<blockquote>AISenseConfig_Hearing_0</blockquote>
</details>
<details>
 <summary>HearingRange</summary>
<blockquote>5000</blockquote>
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

