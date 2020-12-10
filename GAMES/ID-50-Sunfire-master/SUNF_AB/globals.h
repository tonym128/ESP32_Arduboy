#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
//#include <ATMlib.h>
#include <Sprites.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1

#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_PLAYING           6

#define STATE_WAVE_COMPLETE          7
#define STATE_GAME_OVER              8


Arduboy2Base arduboy;
Sprites sprites;

//ATMsynth ATM;

boolean danger = false;
boolean blinking = true;
boolean songPlayed = false;

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;

unsigned long score = 0;



#endif
