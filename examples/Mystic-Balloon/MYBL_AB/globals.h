#ifndef GLOBALS_H
#define GLOBALS_H

/*-----------------------------*
   To turn on hard mode
   uncomment the below define.

   Hard mode makes it so you
   start each level without
   recovering balloons.
  ----------------------------*/
//#define HARD_MODE

#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "vec2.h"
#include "bitmaps.h"

// EEPROM - change this address offset from the arduboy starting address if desired
#define OFFSET_MYBL_START            (EEPROM_STORAGE_SPACE_START + 51)
#define OFFSET_LEVEL                 (OFFSET_MYBL_START + sizeof(uint8_t))
#define OFFSET_COINS                 (OFFSET_LEVEL + sizeof(uint8_t))
#define OFFSET_COINSHS               (OFFSET_COINS + sizeof(uint8_t))
#define OFFSET_SCORE                 (OFFSET_COINSHS + sizeof(uint8_t))
#define OFFSET_HSCORE                (OFFSET_SCORE + sizeof(unsigned long))
#define OFFSET_MYBL_END              (OFFSET_HSCORE + sizeof(unsigned long))

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_NEXT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_OVER              9
#define STATE_GAME_PLAYCONTNEW       10 // 11

#define FACING_RIGHT                 0
#define FACING_LEFT                  1

#define LEVEL_TO_START_WITH          1
#define TOTAL_LEVELS                 39
#define TOTAL_COINS                  TOTAL_LEVELS * 6

#define MAX_PER_TYPE                 6                    // total instances per enemy type

#define LEVEL_WIDTH                  384                  // 24 * 16
#define LEVEL_HEIGHT                 384                  // 24 * 16
#define LEVEL_CELLSIZE               16
#define LEVEL_WIDTH_CELLS            24
#define LEVEL_HEIGHT_CELLS           24
#define LEVEL_CELL_uint8_tS             (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE             576

#define PLAYER_JUMP_TIME             11

// This is a replacement for struct Rect in the Arduboy2 library.
// It defines height as an int16_t instead of a uint8_t to allow a higher rectangle.
struct HighRect
{
  public:
    int16_t x;
    int16_t y;
    uint16_t width;
    int16_t height;
};

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

uint8_t gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
uint8_t menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
uint8_t globalCounter = 0;
uint8_t level;
unsigned long scorePlayer;
uint8_t coinsCollected = 0;
uint8_t totalCoins = 0;
uint8_t balloonsLeft;

boolean nextLevelIsVisible;
boolean scoreIsVisible;
boolean canPressButton;
boolean pressKeyIsVisible;

uint8_t walkerFrame = 0;
uint8_t fanFrame = 0;
uint8_t coinFrame = 0;
uint8_t coinsActive = 0;
vec2 levelExit = vec2(0, 0);
vec2 startPos;
uint8_t mapTimer = 10;

void loadSetEEPROM()
{
  if ((EEPROM.read(OFFSET_MYBL_START) != GAME_ID) && (EEPROM.read(OFFSET_MYBL_END) != GAME_ID))
  {
    EEPROM.put(OFFSET_MYBL_START, (uint8_t)GAME_ID); // game id
    EEPROM.put(OFFSET_LEVEL, (uint8_t)LEVEL_TO_START_WITH - 1); // beginning level
    EEPROM.put(OFFSET_COINS, (uint8_t)0); // coins current run
    EEPROM.put(OFFSET_COINSHS, (uint8_t)0); // coins highscore run
    EEPROM.put(OFFSET_SCORE, (unsigned long)0); // clear score
    EEPROM.put(OFFSET_HSCORE, (unsigned long)0); // clear high score
    EEPROM.put(OFFSET_MYBL_END, (uint8_t)GAME_ID); // game id
  }
}

// This is a replacement for the collide() function in the Arduboy2 library.
// It uses struct HighRect instead of the struct Rect in the library.
bool collide(HighRect rect1, HighRect rect2)
{
  return !( rect2.x                 >=  rect1.x + rect1.width    ||
            rect2.x + rect2.width   <=  rect1.x                ||
            rect2.y                 >=  rect1.y + rect1.height ||
            rect2.y + rect2.height  <=  rect1.y);
}

#endif
