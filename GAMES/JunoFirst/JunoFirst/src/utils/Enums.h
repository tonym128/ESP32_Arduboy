#pragma once

#include "Arduboy2Ext.h"


// How fast should the ground move ?  1 = Slow, 2 = Medium, 3 = Fast - I think 2 is the most appropriate.

#define GROUND_SPEED 3


// You can comment this out (or append it with an underscore or something) to have solid lines ..

#define _TEST_ASTRONAUT
#define _INC_HEALTH   

// Do we invert the screen?
#define USE_INVERT
// Just once or multiple times
#define SINGLE_FLASH


// ---------------------------------------------------------------------
//  You can play with these ..

static const uint8_t ENEMIES_IN_FIRST_WAVE                = 12;         // How many enemies in the first wave?
static const uint8_t ADDITIONAL_ENEMIES_PER_WAVE          = 2;          // How many enemies in each additional wave?

static const uint8_t INITIAL_BULLET_FREQUENCY             = 60;         // How frequently should enemies shoot you?  Higher is less often.
static const uint8_t BULLETS_DECREASE_PER_WAVE            = 2;          // How does the frequency increase each wave?  Bigger numbers means enemies fire more rapidly each wave ..

#if GROUND_SPEED == 1
static const uint8_t DEFAULT_FRAME_RATE                   = 70;         // Frame rate the game starts at.
static const uint8_t NEW_WAVE_FRAME_RATE_INC              = 4;          // Frame rate increment per wave.
#endif

#if GROUND_SPEED == 2
static const uint8_t DEFAULT_FRAME_RATE                   = 60;         // Frame rate the game starts at.
static const uint8_t NEW_WAVE_FRAME_RATE_INC              = 4;          // Frame rate increment per wave.
#endif

#if GROUND_SPEED == 3
static const uint8_t DEFAULT_FRAME_RATE                   = 50;         // Frame rate the game starts at.
static const uint8_t NEW_WAVE_FRAME_RATE_INC              = 4;          // Frame rate increment per wave.
#endif

static const uint8_t FRAME_RATE_DEC_FUEL                  = 60;         // How wuickly does your fuel deplete?  Again, bigger numbers result in slower depletion,
static const uint16_t DOUBLE_UP_POINTS_DELAY              = 500;        // How long do we double up points after an astronaut capture?

#ifdef INC_HEALTH
static const uint16_t INCREMENT_HEALTH                    = 800;        // How quickly does your health regain?  Bigger numbers result in slower regain.
#endif

static const uint8_t HEALTH_COUNTDOWN                     = 6;          // How many times do we flash the screen / red LED when hit?

// ---------------------------------------------------------------------
//  Do not play with these !

static const uint8_t WIDTH_HALF                           = 57;
static const uint8_t VISIBLE_SCREEN_WIDTH                 = 113;

static const uint8_t MAX_NUMBER_OF_ENEMIES                = 12;
static const uint8_t MAX_NUMBER_OF_ENEMIES_PER_FORMATION  = 4;
static const uint8_t MAX_NUMBER_OF_BULLETS                = 10;
static const uint8_t MAX_NUMBER_OF_LIVES                  = 4;

static const uint8_t MAX_NUMBER_OF_SCORES                 = 5;
static const uint8_t DO_NOT_EDIT_SLOT                     = 255;

static const uint8_t HORIZON_MIN_VALUE                    = 42;
static const uint8_t HORIZON_INCREMENT                    = 3;
static const uint8_t HORIZON_COL_COUNT                    = 7;
static const uint8_t HORIZON_ROW_COUNT                    = 5;
static const uint8_t ENEMY_FRAME_COUNT                    = 30;
static const uint8_t ENEMY_FRAME_COUNT_HALF               = 15;

static const uint8_t ENEMY_DISTANCE_HORIZON_START_1       = 24;
static const uint8_t ENEMY_DISTANCE_HORIZON_END_1         = 47;
static const uint8_t ENEMY_DISTANCE_HORIZON_START_2       = 48;
static const uint8_t ENEMY_DISTANCE_HORIZON_END_2         = 51;
static const uint8_t ENEMY_DISTANCE_FAR_START             = 52;
static const uint8_t ENEMY_DISTANCE_FAR_END               = 57;
static const uint8_t ENEMY_DISTANCE_MEDIUM_START          = 58;
static const uint8_t ENEMY_DISTANCE_MEDIUM_END            = 63;
static const uint8_t ENEMY_DISTANCE_CLOSE_START           = 64;
static const uint8_t ENEMY_DISTANCE_CLOSE_END             = 122;

static const uint8_t ENEMY_VISIBLE_HORIZON                = 24;
static const uint8_t INTRO_DELAY                          = 60;

static const int8_t ENEMY_MINIMUM_X                       = -80;
static const int8_t ENEMY_MAXIMUM_X                       = 80;
static const int8_t ENEMY_MINIMUM_Y                       = 0;
static const int8_t ENEMY_MAXIMUM_Y                       = 120;

static const uint8_t ENEMY_DISTANCE_FAR_WIDTH             = 5;
static const uint8_t ENEMY_DISTANCE_MEDIUM_WIDTH          = 8;
static const uint8_t ENEMY_DISTANCE_CLOSE_WIDTH           = 9;

static const uint8_t ENEMY_DISTANCE_FAR_HEIGHT            = 5;
static const uint8_t ENEMY_DISTANCE_MEDIUM_HEIGHT         = 7;
static const uint8_t ENEMY_DISTANCE_CLOSE_HEIGHT          = 13;

static const uint8_t ENEMY_DISTANCE_FAR_WIDTH_HALF        = 2;
static const uint8_t ENEMY_DISTANCE_MEDIUM_WIDTH_HALF     = 3;
static const uint8_t ENEMY_DISTANCE_CLOSE_WIDTH_HALF      = 6;

static const uint8_t FRAME_RATE_1                         = 1;
static const uint8_t FRAME_RATE_2                         = 2;
static const uint8_t FRAME_RATE_3                         = 3;
static const uint8_t FRAME_RATE_4                         = 4;
static const uint8_t FRAME_RATE_8                         = 8;
static const uint8_t FRAME_RATE_12                        = 12;
static const uint8_t FRAME_RATE_16                        = 16;


static const uint16_t FUEL_BONUS_INC                      = 500;        // What bonus do you get for each fuel unit left at the end of a level?
static const uint8_t FUEL_BONUS_INC_DIV                   = 5;          // Above divided by 100.


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}


// ----------------------------------------------------------------------------
//  Clamp the value within the range .. 
//
template <typename T> T clamp(const T& value, const T& low, const T& high) 
{
  return value < low ? low : (value > high ? high : value); 
}

// ----------------------------------------------------------------------------
//  GameState defines the modes of the game ..
//
enum class GameState : uint8_t {

  Intro_Init,
  Intro,
  ScoreTable,
  Wave_Init,
  Wave,
  WaveCompleted_Init,
  WaveCompleted,
  GamePlay,
  GameOver_Init,
  GameOver,
  SaveScore,
  HighScore,
  Credits,
  Paused

};


enum class IntroductionState : uint8_t {

  Start,
  HighScore,
  Credits

};

enum class ImageSize : uint8_t {

  Horizon,
  Far,
  Medium,
  Close

};


enum class EnemyType : uint8_t {

  EnemyType1,
  EnemyType2,
  EnemyType3,
  EnemyType4,
  AstronautCaptured,
  AstronautReleased

};


enum class MovementSequence : uint8_t {

  Sequence_1,       // Move left to right by xDelta.  When at edge of screen move forward yDelta, change x direction.  When at bottom of screen, reverse yDelta.
  Sequence_2,       // Move both X and Y simultaneously.  When at an edge of the screen, reverse direction.  When at bottom of screen reverse direction.
  Sequence_3,       // Move both X and Y simultaneously.  When at an edge of the screen, reverse direction.  When at bottom of screen reappear at top.
  Sequence_4,       // Return to the top of the screen.  When at the top, disable the enemy.  Used for Astronaut.

};


enum class EnemyStatus : uint8_t {

  Dead,
  Explosion1,
  Explosion2,
  Explosion3,
  Explosion4,
  Active,
  Spawn1,
  Spawn2,
  Spawn3,
  Spawn4,
  Spawn5,
  Spawn6,
  Spawn7,
  Spawn8

};


enum class PlayerStatus : uint8_t {

  Dead,
  Dead_Delay_01,
  Dead_Delay_02,
  Dead_Delay_03,
  Dead_Delay_04,
  Dead_Delay_05,
  Dead_Delay_06,
  Dead_Delay_07,
  Dead_Delay_08,
  Dead_Delay_09,
  Dead_Delay_10,
  Explosion1,
  Explosion2,
  Explosion3,
  Explosion4,
  Active

};


// EnemyStatus ..

inline EnemyStatus operator++( EnemyStatus & c ) {

  c = static_cast<EnemyStatus>( static_cast<uint8_t>(c) + 1 );
  return c;

}

inline EnemyStatus operator++( EnemyStatus & c, int ) {

  EnemyStatus result = c;
  ++c;
  return result;

}

inline EnemyStatus operator--( EnemyStatus & c ) {
 
  c = static_cast<EnemyStatus>( static_cast<uint8_t>(c) - 1 );
  return c;

}

inline EnemyStatus operator--( EnemyStatus & c, int ) {

  EnemyStatus result = c;
  --c;
  return result;

}


// PlayerStatus ..

inline PlayerStatus operator++( PlayerStatus & c ) {

  c = static_cast<PlayerStatus>( static_cast<uint8_t>(c) + 1 );
  return c;

}

inline PlayerStatus operator++( PlayerStatus & c, int ) {

  PlayerStatus result = c;
  ++c;
  return result;

}

inline PlayerStatus operator--( PlayerStatus & c ) {
 
  c = static_cast<PlayerStatus>( static_cast<uint8_t>(c) - 1 );
  return c;

}

inline PlayerStatus operator--( PlayerStatus & c, int ) {

  PlayerStatus result = c;
  --c;
  return result;

}


inline IntroductionState operator++( IntroductionState & c ) {

  c = static_cast<IntroductionState>( static_cast<uint8_t>(c) + 1 );
  return c;

}

inline IntroductionState operator++( IntroductionState & c, int ) {

  IntroductionState result = c;
  ++c;
  return result;

}

inline IntroductionState operator--( IntroductionState & c ) {
 
  c = static_cast<IntroductionState>( static_cast<uint8_t>(c) - 1 );
  return c;

}

inline IntroductionState operator--( IntroductionState & c, int ) {

  IntroductionState result = c;
  --c;
  return result;

}