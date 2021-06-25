#pragma once

#include <stdint.h>
#include "Utils.h"

//#define DEBUG
//#define DEBUG_PLAYGAME


// - Overall Settings -----------------

// Delay between fish launches, starts at range between 'min' and 'max' and decreases every 'fish_launch_mod_min' points ..
constexpr uint8_t fish_launch_easy_init_min = 120;
constexpr uint8_t fish_launch_easy_init_max = 160;
constexpr uint8_t fish_launch_easy_mod_min = 4;
constexpr uint8_t fish_launch_easy_mod_max = fish_launch_easy_mod_min * (fish_launch_easy_init_max / fish_launch_easy_init_min);
constexpr uint8_t fish_launch_hard_init_min = 90;
constexpr uint8_t fish_launch_hard_init_max = 120;
constexpr uint8_t fish_launch_hard_mod_min = 3;
constexpr uint8_t fish_launch_hard_mod_max = fish_launch_hard_mod_min * (fish_launch_hard_init_max / fish_launch_hard_init_min);


// Speed at which fish rises, starts at range between 'min' and 'max' and decreases every 'fish_speed_mod_min' points ..
constexpr uint8_t fish_speed_easy_init_min = 20;
constexpr uint8_t fish_speed_easy_init_max = 40;
constexpr uint8_t fish_speed_easy_mod_min = 15;
constexpr uint8_t fish_speed_easy_mod_max = fish_speed_easy_mod_min * (fish_speed_easy_init_max / fish_speed_easy_init_min);
constexpr uint8_t fish_speed_hard_init_min = 16;
constexpr uint8_t fish_speed_hard_init_max = 32;
constexpr uint8_t fish_speed_hard_mod_min = 12;
constexpr uint8_t fish_speed_hard_mod_max = fish_speed_hard_mod_min * (fish_speed_hard_init_max / fish_speed_hard_init_min);

#define RECIPIENT_X_DEFAULT 110
#define RECIPIENT_VISIBLE_LIMIT_MIN 100
#define RECIPIENT_VISIBLE_LIMIT_MAX 140
#define RECIPIENT_REAPPEAR_LIMIT_MIN 60
#define RECIPIENT_REAPPEAR_LIMIT_MAX 100

#define TURTLES_COUNT 5
#define TURTLE_NONE 255
#define FISH_NONE 255 
#define RETURN_TO_LOOKING_DOWN_FROM_UP 60
#define RETURN_TO_LOOKING_DOWN_FROM_DIVING 33


// // - High Scores ---------------------

#define NAME_LENGTH 3
#define NAME_LENGTH_PLUS_TERM (NAME_LENGTH + 1)

#define HS_NAME_LEFT 36
#define HS_SCORE_LEFT 69
#define HS_CHAR_TOP 24
#define HS_CHAR_V_SPACING 9
#define HS_PRESS_A_DELAY 100
#define NO_WINNER 255

#define FLASH_FRAME_COUNT_2 56

enum class Player_Positions : int8_t {
  Standing_Right_ArmsDown,
	Standing_Right_ArmsUp_WithPackage,
  Standing_Left_ArmsDown,
	Standing_Left_ArmsUp_WithPackage,
	Jumping_Right_01,
	Jumping_Right_01_WithPackage,
	Jumping_Right_02,
	Jumping_Right_02_WithPackage,
	Jumping_Left_01,
	Jumping_Left_01_WithPackage,
	Jumping_Left_02,
	Jumping_Left_02_WithPackage,
  Drowning_01,
  Drowning_02,
  Drowning_03,
  Drowning_04,
  Giving,
  Giving_WithPackage,
  Tombstone,
  None
};

enum class GameStateType : uint8_t {
  None,
	SplashScreen,
	TitleScreen,
  GameIntroScreen,
  PlayGameScreen,
  HighScoreScreen
};

enum class Direction : uint8_t {
  Up,     // 00
  Down,   // 01
  Left,   // 02
  Right,  // 03
  None
};

enum class TurtleMode : uint8_t {
  LookingUp,     // 00
  LookingDown,   // 01
  Diving,        // 02
  None
};

enum class TurtleType : uint8_t {
  Black,
  White
};

enum class GameMode : uint8_t {
  Easy,
  Hard
};

struct GameStats {

  public: 
    
    GameStats() { };

    uint16_t score = 0;
    uint8_t numberOfLivesLeft = 3;

    GameMode mode = GameMode::Easy;

    bool gameOver = false;

    void resetGame() {

      this->numberOfLivesLeft = 3;
      this->score = 0; 
      this->gameOver = false;

    }

};