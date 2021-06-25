#ifndef Globals_h
#define Globals_h

#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

// just an or || mSelectedMenuItem == 0 ? 12 : mSelectedMenuItem - 1;

// if this is outcommented it will save 900 bytes pgm-space about 3%
#define USE_FULL_MAP

#define USE_SERIAL

//#define DEBUG_ADD_FUNCTIONS
//#define DEGUG_DMG_PLAYER
//#define DEGUG_DMG_ENEMYS
//#define DEBUG_FRAME_TIME
/*
#ifdef ESP8266
#define PS2_DAT       D6 // brown/green
#define PS2_CMD       D0 // orange 
#define PS2_SEL       D5 // yellow
#define PS2_CLK       D8 // blue
#include <PS2X_lib.h>
PS2X ps2x;
SSD1306Brzo oled(OLED_I2C_ADRESS, D2, D1);
#endif
*/
Arduboy2Base arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define GAME_FRAMES 30

void getButtons() {/*
#ifdef ESP8266
  // this function is called inside arduboy whenever buttonStates() is called
  arduboy.setExternalButtons(ps2x.getArduboyButtons());
#endif*/
}

// this is better not even
#define MOVE_MAP_INTERVAL     3

bool isPlayMode = false;
bool isGameOver = false;
bool isStartAnimation = true;

uint8_t audioMessageTimeout = 0;
uint8_t specialAttackTimeout = 0;
uint16_t slowmoTimeout = 0;

uint32_t nextButtonInput = 0;

uint32_t gameFrames = 0;

uint16_t enemyKills = 0;
uint16_t traveledDist = 0;

enum {
  WEAPON_STDGUN = 0,
  WEAPON_ROCKET,
  WEAPON_LASER,
  WEAPON_MG,
  WEAPON_FLAME,
  WEAPON_SMALL_ROCKET
};

#endif
