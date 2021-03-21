#pragma once

#include "Arduboy2Ext.h"

#define NO_USB
#define _DEBUG

// Comment out IGNORE_BARREL_COLLISIONS to include colllision detection ..
#define _IGNORE_BARREL_COLLISIONS
#define PLAY_SOUNDS


// - Game play stuff ---------------------

#define NUMBER_OF_HOOKS 4
#define INTRO_DELAY_FROM_TITLE 201
#define INTRO_DELAY_NORMAL 200
#define INTRO_MOVE_TO_BOTTOM 100
#define NUMBER_OF_BARRELS_MIN 5
#define NUMBER_OF_BARRELS_MAX 9
#define FRAME_RATE_MIN 67
#define FRAME_RATE_MAX 90

#define NOT_IN_A_POSITION 255
#define INITIAL_BARREL_OFFSET 14

#define GIRDER_COUNT 2
#define NONE_FOUND 255
#define NO_IMAGE 255

#define LEVER_ON_COUNTDOWN 140


// - High Scores ---------------------

#define NAME_LENGTH 3
#define NAME_LENGTH_PLUS_TERM (NAME_LENGTH + 1)

#define HS_NAME_LEFT 36
#define HS_SCORE_LEFT 69
#define HS_CHAR_TOP 24
#define HS_CHAR_V_SPACING 9
#define HS_PRESS_A_DELAY 100
#define NO_WINNER 255

#define FLASH_FRAME_COUNT 40

enum class LeverPosition : uint8_t {
  Off,
  On
};

enum class Stance : uint8_t {
  Normal,                     // 0
  Normal_RHS,                 // 1
  Running_01,                 // 2
  Running_01_RHS,             // 3
  Running_02,                 // 4
  Running_02_RHS,             // 5
  Running_03,                 // 6
  Running_03_RHS,             // 7
  Running_04,                 // 8
  Running_04_RHS,             // 9
  Jump,                       // 8
  Jump_RHS,                   // 9
  Ladder_01,                  // 10
  Ladder_02,                  // 11
  OnCrane,                    // 12
  OnCrane_RHS,                // 13
  Dead_01,                    // 14
  Dead_02,                    // 15
  Dead_03,                    // 16
  OnCrane_EmptyHand,          // 17
  OnCrane_HoldingHook,        // 18
};

enum class GorillaStance : uint8_t {
  Normal,
  Left,
  Right,
  ThrowingBarrel1,
  ThrowingBarrel2,
  ThrowingBarrel3,
  ThrowingBarrel4,
  Dead1,
  Dead2,
  Dead3
};

enum class Components : uint8_t {
  Girder,
  Girder_OverHead, 
  Girder_Small,
  Ladder,
  Plate1,
  Plate2,
  Plate3,
  Lever,
  Cable1,
  Cable2,
  Crane,
  Hook,
  Fire,
  Fire_Foreground,
  EasyHard,
  Dinner,
  Buildings
};

enum class Movements : uint8_t {
  Up = 1,
  Down = 2, 
  Left = 4,
  Right = 8,
  Jump = 16,
  Lever = 32,
  Reverse = 64,
  JumpToCrane = 128
};

enum class Rotation : uint8_t {
  Left = 1,
  Right = 2,
  None = 4
};

enum class GirderRotation : uint8_t {
  Left,
  Right,
  None
};

enum class CranePosition : uint8_t {
  Upright_01 = 0,
  Upright_02 = 1,
  Upright_03 = 2,
  Inclined_01 = 3,
  Inclined_02 = 4,
  Inclined_03 = 5,
  Inclined_04 = 6,
  Inclined_05 = 7,
  Flat = 8,
  Declined = 9,
};

enum class CraneState : uint8_t {
  Dormant,
  TurningOn,
  TurningOff,
  Swinging
};

enum class GameStateType : uint8_t {
  None,
	SplashScreen,
	TitleScreen,
  GameIntroScreen,
  PlayGameScreen,
  HighScoreScreen
};

enum class GameMode : uint8_t {
  Easy,
  Hard
};

struct GameStats {

  public: 
    
    GameStats() { };

    uint16_t score = 0;
    uint8_t numberOfLivesLeft = 1; // 3; //SJH
    uint8_t level = 1;

    GameMode mode = GameMode::Easy;

    bool gameOver = false;

    void resetGame() {

      this->numberOfLivesLeft = 3; //SJH
      this->score = 0; 
      this->level = 1;
      this->gameOver = false;

    }

};