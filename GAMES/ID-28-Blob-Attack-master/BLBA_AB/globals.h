#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO              0
#define STATE_MENU_MAIN               1
#define STATE_MENU_HELP               2
#define STATE_MENU_PLAY               3
#define STATE_MENU_INFO               4
#define STATE_MENU_SOUNDFX            5

//define game states (on main menu)
#define STATE_GAME_PLAYING            6
#define STATE_GAME_PAUSE              7
#define STATE_GAME_OVER               8

#define SPEED_STATE_START             60
#define SPEED_STATE_ONE               55
#define SPEED_STATE_TWO               50
#define SPEED_STATE_THREE             45
#define SPEED_STATE_FOUR              40
#define SPEED_STATE_FIVE              30
#define SPEED_STATE_SIX               20
#define SPEED_STATE_SEVEN             10


Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
unsigned long scorePlayer;
byte currentSpeed;
byte previousSpeed;


#endif
