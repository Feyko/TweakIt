\page BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_Spitter Controller_Spitter
### Parent Class :
[BP_EnemyController](@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyController)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Character/Creature/Enemy/Spitter/Controller_Spitter.Controller_Spitter_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_SpitterPathFollowingComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_SpitterPerceptionComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_SpitterActionsComp

@ref BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_SpitterTransformComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemySpitterController_SpitterRootComponent

## Properties

### mDoPanicName
@htmlonly
<blockquote>DoPanic</blockquote>
@endhtmlonly

### mBlackboardAsset
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_spitter_b_b__spitter.html"><blockquote>BB_Spitter</blockquote></a></b>
</details>
@endhtmlonly

### mBehaviorTree
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_spitter_b_t__spitter.html"><blockquote>BT_Spitter</blockquote></a></b>
</details>
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
<blockquote>1</blockquote>
@endhtmlonly

### mStaticIgnoreCooldown
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mAggroDistanceCurve
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_spitter_curve__spitter_distance_aggro.html"><blockquote>Curve_SpitterDistanceAggro</blockquote></a></b>
</details>
@endhtmlonly

### mGainAggroThreshold
@htmlonly
<blockquote>0.6499999761581421</blockquote>
@endhtmlonly

### mLoseAggroThreshold
@htmlonly
<blockquote>0.5</blockquote>
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
<blockquote>9000</blockquote>
</details>
<details>
 <summary>LoseSightRadius</summary>
<blockquote>11000</blockquote>
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
<blockquote>7500</blockquote>
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

