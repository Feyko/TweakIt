\page BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerElite Controller_StingerElite
### Parent Class :
[BP_EnemyController](@ref BlueprintsGameFactoryGameCharacterCreatureEnemyBP_EnemyController)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Character/Creature/Enemy/Stinger/BigStinger/Controller_StingerElite.Controller_StingerElite_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerElitePathFollowingComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerElitePerceptionComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerEliteActionsComp

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerEliteTransformComponent

@ref BlueprintsGameFactoryGameCharacterCreatureEnemyStingerBigStingerController_StingerEliteRootComponent

## Properties

### mBlackboardAsset
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_a_i_b_b__stinger.html"><blockquote>BB_Stinger</blockquote></a></b>
</details>
@endhtmlonly

### mBehaviorTree
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_big_stinger_b_t__stinger_elite.html"><blockquote>BT_StingerElite</blockquote></a></b>
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
<blockquote>0.10000000149011612</blockquote>
@endhtmlonly

### mStaticIgnoreCooldown
@htmlonly
<blockquote>5</blockquote>
@endhtmlonly

### mAggroDistanceCurve
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_a_i_stinger_distance_aggro.html"><blockquote>StingerDistanceAggro</blockquote></a></b>
</details>
@endhtmlonly

### mGainAggroThreshold
@htmlonly
<blockquote>0.20000000298023224</blockquote>
@endhtmlonly

### mLoseAggroThreshold
@htmlonly
<blockquote>0.15000000596046448</blockquote>
@endhtmlonly

### mAttackPattern
@htmlonly
<ol>
<li>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_attack__stinger_jump.html"><blockquote>Attack_StingerJump</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_attack__stinger_swipe.html"><blockquote>Attack_StingerSwipe</blockquote></a></b>
</li>
<li>
<b><a href="_blueprints_game_factory_game_character_creature_enemy_stinger_attack__stinger_jump.html"><blockquote>Attack_StingerJump</blockquote></a></b>
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
<blockquote>9000</blockquote>
</details>
<details>
 <summary>LoseSightRadius</summary>
<blockquote>10500</blockquote>
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
<blockquote>10000</blockquote>
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

