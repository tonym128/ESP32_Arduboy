#pragma once

#include <stdint.h>
#include "Utils.h"

enum DungeonDecorations : uint8_t {
  BricksLHS = 1,
  BricksMID = 2,
  BricksRHS1 = 4,
  BricksRHS2 = 8,
  Pumpkin = 16,
  Chest = 32,
  Skull = 64,
};

enum DamageType : uint8_t {
  FireBlaze = 1,
  VenomMist
};

enum class InventoryMode : uint8_t {
  Inventory,
  Statistics,
  Runes,
  Enemy
};

inline InventoryMode &operator++( InventoryMode &c ) { 
  c = ( c == InventoryMode::Enemy ) ? InventoryMode::Inventory : static_cast<InventoryMode>( static_cast<uint8_t>(c) + 1 );
  return c;
}

inline InventoryMode operator++( InventoryMode &c, int ) {
  InventoryMode result = c;
  ++c;
  return result;
}

inline InventoryMode &operator--( InventoryMode & c ) {
  c = ( c == InventoryMode::Inventory ) ? InventoryMode::Enemy : static_cast<InventoryMode>( static_cast<uint8_t>(c) - 1 );
  return c;
}

inline InventoryMode operator--( InventoryMode & c, int ) {
  InventoryMode result = c;
  --c;
  return result;
}

enum class ViewState : uint8_t {
  InTown,
  InShopItems,
  InShopItems_ConfirmPurchase,
  InShopRunes,
  InShopRunes_ConfirmPurchase,
  InDungeon_Entry,
  InDungeon_PlayersTurn,
  InDungeon_EnemysAttacksFirst,
  InDungeon_EnemysTurn,
  InDungeon_PlayerDead,
  InDungeon_EnemyDead,
};

enum class GameStateType : uint8_t {
	SplashScreen_Activate,
	SplashScreen,
	TitleScreen_Activate,
	TitleScreen,
  PlayGame_Activate,
  PlayGame,
};

enum class ShopType : uint8_t {
  Items,
  Runes
};

enum class ItemType : uint8_t {
  Tree_01,
  Tree_02,
  Crates,
  Castle_01,
  Castle_02, 
  Tombstone,
  NPC,
  Pumpkin,
  SavingPost,
  HP
};

enum class PlayerDirection : uint8_t {
  IdleLeft,
  IdleRight,
  WalkingLeft,
  WalkingRight
};

enum class PlayerItemType : uint8_t {
  Helmet,
  Potion,
  Exit // used when displaying exit sign.
};

enum class HelmetType : uint8_t {
  Crystal,
  High,
  Round,
  Count,
  None,
};

enum class PotionType : uint8_t {
  Fire,
  Health,
  Speed,
  Count,
  None,
};

enum class TimeOfDay : uint8_t {
  Day,
  Night,
};


enum class RenderPosition : uint8_t {
  Background,
  FrontOfBuildings,
  FrontOfPlayer
};


enum class TreeType : uint8_t {
  Tree_01,
  Tree_02,
};


enum class EnemyType : uint8_t {
  Skeleton,
  SmallWerewolf,
  GelatinousCube,
  LargeWerewolf,
  Astarok
};

inline EnemyType &operator++( EnemyType &c ) { 
  c = ( c == EnemyType::Astarok ) ? EnemyType::Skeleton : static_cast<EnemyType>( static_cast<uint8_t>(c) + 1 );
  return c;
}

inline EnemyType operator++( EnemyType &c, int ) {
  EnemyType result = c;
  ++c;
  return result;
}

inline EnemyType &operator--( EnemyType & c ) {
  c = ( c == EnemyType::Skeleton ) ? EnemyType::Astarok : static_cast<EnemyType>( static_cast<uint8_t>(c) - 1 );
  return c;
}

inline EnemyType operator--( EnemyType & c, int ) {
  EnemyType result = c;
  --c;
  return result;
}

enum class RuneCombinationType_Purchased : uint8_t {
  FireBlaze,
  HealingWind,
  RisingStar,
  VenomMist,
  Count
};

enum RuneCombinationType_Standard {
  Start = 51,
  FiveOfAKind = 51,
  FourOfAKind,
  ThreeOfAKind,
  TwoPairs,
  FullHouse,
  End
};