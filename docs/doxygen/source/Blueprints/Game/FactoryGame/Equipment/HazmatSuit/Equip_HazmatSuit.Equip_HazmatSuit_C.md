\page BlueprintsGameFactoryGameEquipmentHazmatSuitEquip_HazmatSuit Equip_HazmatSuit
### Parent Class :
[FGSuitBase](@ref ClassScriptFGSuitBase)
### Asset Path :
<b><blockquote>/Game/FactoryGame/Equipment/HazmatSuit/Equip_HazmatSuit.Equip_HazmatSuit_C</blockquote></b>
## Components

@ref BlueprintsGameFactoryGameEquipmentHazmatSuitEquip_HazmatSuitDefaultSceneRoot

## Properties

### mImmunity
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

### mIsWorking
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

### mHasNegatedDamage
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

### mDamageNegated
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

### mFilterDuration
@htmlonly
<blockquote>240</blockquote>
@endhtmlonly

### mCountdown
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

### mDisableEffectTimer
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

### mSuit1PMeshMaterials
@htmlonly
<ol>
<li>
<details>
 <summary>SlotName</summary>
<blockquote>Body_Details</blockquote>
</details>
<details>
 <summary>Material</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_player_material_m_i__haz__body__details.html"><blockquote>MI_Haz_Body_Details</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SlotName</summary>
<blockquote>Body_01</blockquote>
</details>
<details>
 <summary>Material</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_player_material_m_i__haz__body_01.html"><blockquote>MI_Haz_Body_01</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SlotName</summary>
<blockquote>Body_02</blockquote>
</details>
<details>
 <summary>Material</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_player_material_m_i__haz__body_02.html"><blockquote>MI_Haz_Body_02</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SlotName</summary>
<blockquote>Body_Hands</blockquote>
</details>
<details>
 <summary>Material</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_player_material_m_i__haz__body__hands.html"><blockquote>MI_Haz_Body_Hands</blockquote></a></b>
</details>
</details>
</li>
<li>
<details>
 <summary>SlotName</summary>
<blockquote>Body_Backpack</blockquote>
</details>
<details>
 <summary>Material</summary>
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_character_player_material_m_i__haz__body__backpack.html"><blockquote>MI_Haz_Body_Backpack</blockquote></a></b>
</details>
</details>
</li>
</ol>
@endhtmlonly

### mAttachmentClass
@htmlonly
<b><a href="_blueprints_game_factory_game_equipment_hazmat_suit_attach__hazmat_suit.html"><blockquote>Attach_HazmatSuit</blockquote></a></b>
@endhtmlonly

### mEquipmentSlot
@htmlonly
<blockquote>EEquipmentSlot::ES_BACK</blockquote>
@endhtmlonly

### mEquipSound
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_equipment_hazmat_suit_audio_play__c__haz_mat_suit__equip.html"><blockquote>Play_C_HazMatSuit_Equip</blockquote></a></b>
</details>
@endhtmlonly

### mUnequipSound
@htmlonly
<details>
 <summary>$AssetPath</summary>
<b><a href="_blueprints_game_factory_game_equipment_hazmat_suit_audio_play__c__haz_mat_suit__un_equip.html"><blockquote>Play_C_HazMatSuit_UnEquip</blockquote></a></b>
</details>
@endhtmlonly

### mCostToUse
@htmlonly
<ol>
<li>
<details>
 <summary>ItemClass</summary>
<b><a href="_blueprints_game_factory_game_resource_parts_iodine_infused_filter_desc__hazmat_filter.html"><blockquote>Desc_HazmatFilter</blockquote></a></b>
</details>
<details>
 <summary>amount</summary>
<blockquote>1</blockquote>
</details>
</li>
</ol>
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
<blockquote>False</blockquote>
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

### bOnlyRelevantToOwner
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### bNetUseOwnerRelevancy
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### bReplicates
@htmlonly
<blockquote>True</blockquote>
@endhtmlonly

### RemoteRole
@htmlonly
<blockquote>1</blockquote>
@endhtmlonly

