
#pragma once

struct PlayerItem {
  PlayerItemType itemType;
  uint8_t itemIndex;
  uint8_t defence;
  uint8_t gold;
};

struct LevelData {
  EnemyType enemyType;
  uint8_t def;
  uint8_t hp;
  uint8_t minAttack;
  uint8_t maxAttack;
  uint8_t minSP;
  uint8_t maxSP;
  uint8_t minGP;
  uint8_t maxGP;
  uint8_t decorations;
};

struct Message {
  uint8_t lines;
  uint8_t width;
  uint8_t victoryGP;
  uint8_t victorySP;
  uint8_t topLine;
  uint8_t linesOverall;
  uint8_t arrayIndex;
  bool renderRequired;
  uint8_t message;
};

struct Inventory {
  uint8_t topIndex;
  uint8_t cursorIndex;
  uint8_t counter; 
  uint8_t returnVal;
  uint8_t runeIdx;
  uint8_t saveGameIndex;
  bool show;
  bool highlight_UpArrow;
  bool highlight_DownArrow;
  InventoryMode mode;
};

struct World {
  
  static constexpr uint8_t townItems_Size = 12;
  static constexpr uint8_t buildings_Size = 2;

  int16_t cloudsXPos;
  int16_t buildingXPos;
  int16_t foregroundXPos;
  uint8_t dungeon;

  uint8_t buildings[buildings_Size];
  TownItem townItems[townItems_Size];

};

struct SplashScreenStateVars {
  uint8_t counter;
};

struct TitleScreenStateVars {
  uint8_t restart;
  uint8_t counter;
  uint8_t soundCounter;
  uint8_t soundIndex;
};

struct PlayGameStateVars {

  uint8_t hasBeenSaved;  // Must be first!
  uint8_t reroll;
  uint8_t counter;
  uint8_t runeIdx;
  uint8_t itemIdx;
  uint8_t invert;       // Counter to invert the screen on inflicting damage ..
  uint8_t numberOfRecasts;

  bool highlight_UpArrow;
  bool highlight_DownArrow;

  ViewState viewState;
    
  uint8_t dmgInflictedOnPlayer;
  uint8_t victoryGP;
  uint8_t victorySP;
  uint8_t victoryFrame;
  bool gameOver;
  bool showSaveDialogue;

  Enemy enemy;
  Runes runes;
  Player player;

  Message message;

  MatchedRuneCombination matchedRuneCombination;
  World world;
  Inventory inventory;

};

struct GameOverStateVars {
  uint8_t winner;
  uint8_t counter;
  uint8_t index;
};

