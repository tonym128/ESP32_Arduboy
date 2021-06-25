#pragma once

#include <stdint.h>
#include "Utils.h"

//#define DEBUG
//#define DEBUG_PLAYGAME


// - Overall Settings -----------------

// #define TARGET_SCORE_BASE 10  
// #define TARGET_SCORE_INC_PER_LEVEL 5 
#define TARGET_SCORE_BASE 45 
#define TARGET_SCORE_INC_PER_LEVEL 5 
#define LED_BRIGHTNESS 32


// - Race Game ------------------------

#define DIST_MAXIMUM 3200 
#define DIST_INTERVAL DIST_MAXIMUM / 10
#define DIST_NO_NEW_CARS 300
#define DIST_GO_TO_HOSPITAL 128
#define DIST_DELAY_AFTER_AMBULANCE_LEAVES_SCREEN 200

#define RACE_PLAYER_HEALTH_MAX 145 
#define RACE_PLAYER_HEALTH_DEC 6 
// #define RACE_PLAYER_HEALTH_MAX 15
// #define RACE_PLAYER_HEALTH_DEC 6 

#define OTHER_CAR_LAUNCH_MAX 400
#define OTHER_CAR_LAUNCH_MAX_DEC 10
#define OTHER_CAR_LAUNCH_MAX_FLOOR 180

#define OTHER_CAR_LAUNCH_MIN 200
#define OTHER_CAR_LAUNCH_MIN_DEC 5
#define OTHER_CAR_LAUNCH_MIN_FLOOR 100

#define RACE_AMBULANCE_WIDTH 31
#define RACE_OTHERCAR_WIDTH 30
#define RACE_OTHERCARS_MAX 3


// - Fire Game ------------------------

#define ANGEL_MISS_1_LEFT 76
#define ANGEL_MISS_2_LEFT 63
#define ANGEL_MISS_3_LEFT 50
#define ANGEL_MISS_TOP 1

#define VICTIMS_MAX_NUMBER 4

#define VICTIM_ON_GROUND_1 43
#define VICTIM_ON_GROUND_2 50
#define VICTIM_HALO_DELAY 4
#define VICTIM_HALO_MAX 60
#define VICTIM_MISSED_TRAMPOLINE 1
#define VICTIM_BOUNCE_HEIGHT 37
#define VICTIM_WIDTH_HALF 8
#define VICTIM_IN_AMBULANCE 102

#define VICTIM_COUNTDOWN 180
#define VICTIM_COUNTDOWN_NONE 65535
#define VICTIM_NONE_AVAILABLE 255

#define PLAYER_MIN_X_POS 18
#define PLAYER_MID_X_POS 39
#define PLAYER_MAX_X_POS 60
#define PLAYER_WIDTH_HALF 20

#define ACCURACY_TOLERANCE 10

#define PLAYER_RANGE1_X_LEFT_START_POS PLAYER_MIN_X_POS
#define PLAYER_RANGE1_X_LEFT_END_POS PLAYER_RANGE1_X11_POS + 3
#define PLAYER_RANGE1_X_MID_START_POS PLAYER_MIN_X_POS
#define PLAYER_RANGE1_X_MID_END_POS PLAYER_RANGE1_X11_POS + 3
#define PLAYER_RANGE1_X_RIGHT_START_POS PLAYER_MID_X_POS - 4
#define PLAYER_RANGE1_X_RIGHT_END_POS PLAYER_MID_X_POS - 1

#define PLAYER_RANGE1_X_21_POS PLAYER_MID_X_POS
#define PLAYER_RANGE1_X_22_POS PLAYER_RANGE1_X21_POS + 3
#define PLAYER_RANGE1_X_23_POS PLAYER_MAX_X_POS - 4
#define PLAYER_RANGE1_X_24_POS PLAYER_MAX_X_POS - 1

#define PLAYER_Y_POS 42
#define PLAYER_STEP_INC 6 


// - High Scores ---------------------

#define NAME_LENGTH 3
#define NAME_LENGTH_PLUS_TERM (NAME_LENGTH + 1)

#define HS_NAME_LEFT 36
#define HS_SCORE_LEFT 69
#define HS_CHAR_TOP 22
#define HS_CHAR_V_SPACING 9
#define HS_PRESS_A_DELAY 100

#define NO_WINNER 255

constexpr size_t eepromStart = 120;
constexpr size_t eepromStart_C1 = eepromStart + 1;
constexpr size_t eepromStart_C2 = eepromStart + 2;
constexpr size_t eepromSaveEntriesCount = 3;
constexpr size_t eepromSaveEntriesStart = eepromStart + 3;

#define FLASH_FRAME_COUNT_2 56
#define CAR_COLLISION_NONE 255

const int8_t edgePos[] = { 5, 0, 22, 1, 5, 16, 22, 17, 5, 31, 22, 32 };
const uint8_t cloud_X_Pos[] = { 16, 16, 18, 18, 46 };
const uint8_t cloud_Y_Pos[] = { 10, 7, 0, 0, 0 };
const uint8_t angel_miss_X_Pos[] = { ANGEL_MISS_1_LEFT, ANGEL_MISS_2_LEFT, ANGEL_MISS_3_LEFT };

enum class TimeOfDay : uint8_t {
  Day,
  Night,
  Mixed
};

enum class GameStateType : uint8_t {
  None,
	SplashScreen,
	TitleScreen,
  GameIntroScreen,
  GameIntroScreen_ChangeDay,
  PlayGameScreen,
  PlayRaceScreen,
  HighScoreScreen
};
  
enum class LightsState : uint8_t {
  Lights_1,
  Lights_2
};


enum class Direction : uint8_t {
  Up,     // 00
  Down,   // 01
  Left,   // 02
  Right,  // 03
  None
};

struct GameStats {

  public: 
    
    GameStats() { };

    uint16_t score = 0;
    uint8_t misses = 0;
    int16_t health = 0;
    uint16_t level = 0;
    uint8_t xPosition = 0;  // Player x position between sequences ..
    uint16_t targetScore = 0;
    bool gameOver = false;

    TimeOfDay timeOfDay = TimeOfDay::Day;

    void resetGame() {

      this->level = 0;
      this->health = 0;
      this->misses = 0;
      this->score = 0; 
      this->timeOfDay = TimeOfDay::Day;
      this->gameOver = false;

    }

};