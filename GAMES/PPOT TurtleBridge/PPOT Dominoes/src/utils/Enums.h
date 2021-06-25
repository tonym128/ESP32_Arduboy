#pragma once

#include "Arduboy2Ext.h"

#define SOUNDS
#define ANIMATIONS

#define _DEBUG
  #ifdef DEBUG
    #define _DEBUG_PLAYER_CANNOT_PLAY
    #define _DEBUG_PLAYER_CAN_FINISH_GAME
    #define _DEBUG_PLAYER_A_FEW_TILES_EACH
    #define _DEBUG_COMPUTER_CAN_FINISH_GAME
    #define _DEBUG_NO_ONE_CAN_PLAY_1
    #define _DEBUG_NO_ONE_CAN_PLAY_2
    #define _DEBUG_CORRUPTED_GRAPHICS
    #define _DEBUG_ALLSIDES_CANNOT_PLAY
  #endif


#define SCORE_COMPUTER_LEFT                   0
#define SCORE_COMPUTER_TOP                    0
#define SCORE_COMPUTER_WIDTH                  16
#define SCORE_COMPUTER_HEIGHT                 7

#define SCORE_PLAYER_LEFT                     SCORE_COMPUTER_LEFT
#define SCORE_PLAYER_TOP                      44
#define SCORE_PLAYER_WIDTH                    SCORE_COMPUTER_WIDTH
#define SCORE_PLAYER_HEIGHT                   SCORE_COMPUTER_HEIGHT

#define BONES_COUNT                           28
#define BONES_INITIAL_COUNT                   7

#define BONES_SMALL_X_SPACING_PORTRAIT        10
#define BONES_SMALL_Y_SPACING_PORTRAIT        18
#define BONES_SMALL_X_SPACING_LANDSCAPE       BONES_SMALL_Y_SPACING_PORTRAIT

#define BONE_C_POS_X_NO_BONES_PLAYED          19
#define BONE_C_POS_Y_NO_BONES_PLAYED          23

#define BONE_W_POS_X_ONE_BONE_PLAYED          1
#define BONE_C_POS_X_ONE_BONE_PLAYED          BONE_C_POS_X_NO_BONES_PLAYED
#define BONE_E_POS_X_ONE_BONE_PLAYED          37
#define BONE_W_POS_Y_ONE_BONE_PLAYED          BONE_C_POS_Y_NO_BONES_PLAYED
#define BONE_C_POS_Y_ONE_BONE_PLAYED          BONE_C_POS_Y_NO_BONES_PLAYED
#define BONE_E_POS_Y_ONE_BONE_PLAYED          BONE_C_POS_Y_NO_BONES_PLAYED

#define BONE_W_POS_X_TWO_BONES_PLAYED         9
#define BONE_C_POS_X_TWO_BONES_PLAYED         28
#define BONE_W_POS_Y_TWO_BONES_PLAYED         23 
#define BONE_C_POS_Y_TWO_BONES_PLAYED         23

#define BONE_N_POS_X_DOUBLE_BONE_PLAYED       23 
#define BONE_S_POS_X_DOUBLE_BONE_PLAYED       23
#define BONE_E_POS_X_DOUBLE_BONE_PLAYED       BONE_E_POS_X_ONE_BONE_PLAYED
#define BONE_W_POS_X_DOUBLE_BONE_PLAYED       BONE_W_POS_X_ONE_BONE_PLAYED
#define BONE_C_POS_X_DOUBLE_BONE_PLAYED       BONE_C_POS_X_ONE_BONE_PLAYED
#define BONE_N_POS_Y_DOUBLE_BONE_PLAYED       5
#define BONE_C_POS_Y_DOUBLE_BONE_PLAYED       BONE_C_POS_Y_NO_BONES_PLAYED
#define BONE_S_POS_Y_DOUBLE_BONE_PLAYED       33
#define BONE_W_POS_Y_DOUBLE_BONE_PLAYED       BONE_C_POS_Y_NO_BONES_PLAYED
#define BONE_E_POS_Y_DOUBLE_BONE_PLAYED       BONE_C_POS_Y_NO_BONES_PLAYED

#define BONE_N_POS_X_AXIS_PLAYED              BONE_N_POS_X_DOUBLE_BONE_PLAYED 
#define BONE_S_POS_X_AXIS_PLAYED              BONE_S_POS_X_DOUBLE_BONE_PLAYED
#define BONE_E_POS_X_AXIS_PLAYED              BONE_E_POS_X_DOUBLE_BONE_PLAYED
#define BONE_W_POS_X_AXIS_PLAYED              BONE_W_POS_X_DOUBLE_BONE_PLAYED
#define BONE_C_POS_X_AXIS_PLAYED              BONE_C_POS_X_DOUBLE_BONE_PLAYED
#define BONE_N_POS_Y_AXIS_PLAYED              BONE_N_POS_Y_DOUBLE_BONE_PLAYED
#define BONE_C_POS_Y_AXIS_PLAYED              BONE_C_POS_Y_DOUBLE_BONE_PLAYED
#define BONE_S_POS_Y_AXIS_PLAYED              BONE_S_POS_Y_DOUBLE_BONE_PLAYED
#define BONE_W_POS_Y_AXIS_PLAYED              BONE_W_POS_Y_DOUBLE_BONE_PLAYED
#define BONE_E_POS_Y_AXIS_PLAYED              BONE_E_POS_Y_DOUBLE_BONE_PLAYED

#define BONES_GRAVEYARD_X                     58
#define BONES_GRAVEYARD_X_MAX                 127
#define BONES_GRAVEYARD_Y                     0
#define BONES_GRAVEYARD_Y_MAX                 53
#define BONES_GRAVEYARD_BONES_PER_ROW         7
#define BONES_GRAVEYARD_BONES_ROWS            3

#define PLAYER_BOTTOM_HAND_X                  10
#define PLAYER_BOTTOM_HAND_Y                  55
#define PLAYER_HAND_MAX_VISIBLE               6
#define GAME_FIRST_TO_REACH_SCORE             100

#define PLAYER_BOTTOM_VISIBLE                 PLAYER_HAND_MAX_VISIBLE
#define PLAYER_BOTTOM_LEFT_ARROW_X            0
#define PLAYER_BOTTOM_LEFT_ARROW_Y            55
#define PLAYER_BOTTOM_LEFT_ARROW_WIDTH        5
#define PLAYER_BOTTOM_LEFT_ARROW_HEIGHT       9
#define PLAYER_BOTTOM_RIGHT_ARROW_X           122
#define PLAYER_BOTTOM_RIGHT_ARROW_Y           PLAYER_BOTTOM_LEFT_ARROW_Y
#define PLAYER_BOTTOM_RIGHT_ARROW_WIDTH       PLAYER_BOTTOM_LEFT_ARROW_WIDTH
#define PLAYER_BOTTOM_RIGHT_ARROW_HEIGHT      PLAYER_BOTTOM_LEFT_ARROW_HEIGHT

#define BOARD_MODE_NO_BONES_PLAYED            0
#define BOARD_MODE_ONE_BONE_PLAYED            1
#define BOARD_MODE_TWO_BONES_PLAYED           2
#define BOARD_MODE_DOUBLE_BONE_PLAYED         3
#define BOARD_MODE_X_AXIS_PLAYED              4

#define UPLOAD_DELAY                          16
#define NOTHING                               255
#define NAME_LENGTH                           9
#define NAME_CHARACTER_SPACING                5
#define NAME_UNDERLINE_WIDTH                  3

#define ASCII_SPACE 32
#define ASCII_CAPITAL_A 65
#define ASCII_CAPITAL_B 66
#define ASCII_CAPITAL_Y 89
#define ASCII_CAPITAL_Z 90
#define ASCII_LOWER_A 97
#define ASCII_LOWER_B 98
#define ASCII_LOWER_Y 121
#define ASCII_LOWER_Z 122

#define EEPROM_START                  400
#define EEPROM_START_C1               401
#define EEPROM_START_C2               402
#define EEPROM_PLAYER1                403
#define EEPROM_PLAYER2                413
#define EEPROM_PLAYER3                423
#define EEPROM_HS_NAME_1              443
#define EEPROM_HS_NAME_2              453
#define EEPROM_HS_NAME_3              463
#define EEPROM_HS_NAME_4              473
#define EEPROM_HS_NAME_5              483
#define EEPROM_HS_SCORE_1             493
#define EEPROM_HS_SCORE_2             495
#define EEPROM_HS_SCORE_3             497
#define EEPROM_HS_SCORE_4             499
#define EEPROM_HS_SCORE_5             501
#define EEPROM_END                    503

#define FLASH_FRAME_COUNT_2                   56
#define KEY_REPEAT_DELAY                      128
#define NO_WINNER 255

enum class TitleMode : uint8_t {
  PressA,
  EnterNumberOfPlayers,
};

enum Position : uint8_t {
  Center,
  North,
  South,
  East,
  West,
  Nothing = 255
};

enum Player : uint8_t {
  Computer,
  Human_1,
  Human_2,
  None
};

enum class GameMode : uint8_t {
  SinglePlayer,
  TwoPlayer
};

enum class HandPosition : uint8_t {
  Top,
  Bottom
};

enum class GameState : uint8_t {
  SplashScreen,
  TitleScreen,
  GameOver,
  GetNames,
  PlayGame,
  PlayComputer,
  SwapPlayers,
  HighScore,
  PlayHuman_HandSel,
  PlayHuman_GraveSel,
  PlayHuman_BoardSel
};




// Extract individual digits of a uint8_t -------------------------------------

template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint8_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// Extract individual digits of a uint16_t
template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }
  
}


// ----------------------------------------------------------------------------
//  Swap two elements .. 
//
template <typename T> void swap(T& x,T& y) {
	T temp;
	temp = x;
	x = y;
	y = temp;
}


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}