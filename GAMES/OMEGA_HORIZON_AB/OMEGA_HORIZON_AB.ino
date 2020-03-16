/*
  Omega Horizon: http://www.neo-retro.games#game_3
  Arduboy version 1.3
  Copyright (C) Neo Retro Games - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
  2017 - SHDWWZRD & KURONOWACHAN

0.2
-bug fix - enemies can no longer leave the screen

0.3
-mission, mission complete, story, and game over screens
-add sound icon to title screen
-update toom 11 and 19

0.4
-updated room 11
-added sound effects and title screen music

1.0-28034/28672
-player can now jump off the back on an enemy
 
1.1-27310/28672 - 1362 open
-remove unused code to shrink game size, shrunk the objects array to actual size

1.2
-added Omega's ship the Omega One landing on planet Turnax
-story screen appears one letter at a time
-added creators to title screen

1.3
-changed how sound status is accessed in eeprom

*/
//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Arduboy2.h>
#include "Tinyfont.h"//830 PROGMEM - 28 RAM
#include <ArduboyTones.h>//626 PROGMEM - 30 RAM

Arduboy2Base arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Sprites sprites;
Tinyfont tinyfont = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());

#define GAME_ID 3

//define menu states (on main menu)
#define STATE_MENU_INTRO         0 //show group logo
#define STATE_TITLE_SCREEN       1 //play, sound on/off, music on/off SHDWWZRD in the corner
#define STATE_STORY_SCREEN       2
#define STATE_MISSION_SCREEN     3
#define STATE_PLANET_SCREEN      4
#define STATE_GAME_PLAYING       5 //main game playing screen
#define STATE_ROOM_TRANSITION    6 
#define STATE_GAME_OVER          7 //game over text on screen, show score and level reached returns to title screen after a button clicked
#define STATE_MISSION_COMPLETE   8

//player defines
#define VERSION "1.3"

/////////////////////////////////////////////////////////////////////////////////
//IMAGES
/////////////////////////////////////////////////////////////////////////////////

const unsigned char PROGMEM neoretrologo2[] =
{
// width, height,
64, 64,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x70, 0x77, 0x77, 0x07, 0x00, 0x07, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x07, 0x07, 0x77, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0xdc, 0xdc, 0xdc, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0xdc, 0xdc, 0xdc, 0x00, 0x00, 0x70, 0x77, 0x77, 0x07, 0x00, 0x07, 0x77, 0x77, 0x70, 0x00, 0x00, 
0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x77, 0x70, 0x71, 0x01, 0x01, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x77, 0x70, 0x71, 0x01, 0x01, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 
0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM logo[] =
{
// width, height,
40, 64,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0x70, 0x70, 0x70, 0x70, 0xf0, 0xf0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x07, 0x03, 0x11, 0x40, 0x60, 0x00, 0x00, 0x60, 0x40, 0x11, 0x03, 0x07, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x02, 0x03, 0x03, 0x04, 0x3f, 0x7c, 0xfc, 0xfc, 0xff, 0xfc, 0xe3, 0x03, 0x00, 0x23, 0x0b, 0x1b, 0x00, 0x00, 0x1b, 0x0b, 0x23, 0x00, 0x03, 0xe3, 0xfc, 0xff, 0xfc, 0xfc, 0x7c, 0x3f, 0x04, 0x03, 0x03, 0x02, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xf0, 0xe0, 0xc0, 0xc1, 0xc3, 0xe3, 0xe7, 0xff, 0xfe, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0xff, 0xe7, 0xe3, 0xc3, 0xc1, 0xc0, 0xe0, 0xe0, 0xf0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x04, 0xf4, 0x14, 0xf4, 0x04, 0xf4, 0x24, 0x44, 0x24, 0xf4, 0x04, 0xf4, 0x54, 0x14, 0x04, 0xf4, 0x14, 0xd4, 0x04, 0xf4, 0x94, 0xf4, 0x04, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfe, 0x02, 0xfa, 0x22, 0xfb, 0x00, 0xf9, 0x89, 0xf9, 0x00, 0xf9, 0x68, 0xb8, 0x40, 0x01, 0x88, 0xf9, 0x89, 0x01, 0xc8, 0xa9, 0x99, 0x01, 0xf8, 0x89, 0xf8, 0x01, 0xf8, 0x33, 0x42, 0xfa, 0x02, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x82, 0xc2, 0x62, 0xb2, 0x52, 0x52, 0x52, 0x52, 0xb2, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xc2, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x30, 0x6f, 0xd0, 0xa6, 0xa9, 0xa9, 0xa0, 0xd0, 0x6f, 0x40, 0x5d, 0x55, 0x53, 0x40, 0x5f, 0x51, 0x5f, 0x40, 0x40, 0x5f, 0x40, 0x40, 0x41, 0x41, 0x5f, 0x40, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM creators[] =
{
// width, height,
128, 8,
// FRAME 00
0xb8, 0xa8, 0xe8, 0x00, 0xf8, 0x20, 0xf8, 0x00, 0xf8, 0x88, 0x70, 0x00, 0xf8, 0x40, 0xf8, 0x00, 0xf8, 0x40, 0xf8, 0x00, 0xc8, 0xa8, 0x98, 0x00, 0xf8, 0x68, 0xb8, 0x00, 0xf8, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x20, 0xd8, 0x00, 0xf8, 0x80, 0xf8, 0x00, 0xf8, 0x68, 0xb8, 0x00, 0xf8, 0x88, 0xf8, 0x00, 0xf8, 0x70, 0xf8, 0x00, 0xf8, 0x88, 0xf8, 0x00, 0xf8, 0x40, 0xf8, 0x00, 0xf8, 0x28, 0xf8, 0x00, 0xf8, 0x88, 0x88, 0x00, 0xf8, 0x20, 0xf8, 0x00, 0xf8, 0x28, 0xf8, 0x00, 0xf8, 0x70, 0xf8,
};

const unsigned char PROGMEM sound_icon[] =
{
// width, height,
8, 8,
// FRAME 00
0xff, 0x9f, 0x83, 0xf9, 0xcd, 0xc1, 0xff, 0xff,
};

const unsigned char PROGMEM omega_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x04, 0x8e, 0x84, 0xee, 0xe4, 0xff, 0x13, 0xff, 
0xb5, 0xff, 0xc6, 0xff, 0x00, 0xc6, 0x00, 0x00, 

// FRAME 01
0x04, 0x0e, 0x04, 0x0e, 0x04, 0xbf, 0xb3, 0xff, 
0xf5, 0xff, 0x26, 0xff, 0x60, 0xfe, 0x00, 0x60, 

// FRAME 02
0x04, 0x0e, 0x04, 0x6e, 0x64, 0xff, 0xd3, 0xff, 
0xf5, 0xff, 0x06, 0xff, 0x00, 0x06, 0x00, 0x00, 

// FRAME 03
0x04, 0x0e, 0x04, 0x2e, 0x24, 0x7f, 0x53, 0xff, 
0x35, 0xff, 0xc6, 0xff, 0x00, 0xc6, 0x00, 0x00, 

// FRAME 04
0x00, 0x00, 0x00, 0xc6, 0xc6, 0xff, 0xb5, 0xff, 
0x13, 0xff, 0xe4, 0xff, 0x84, 0xee, 0x04, 0x8e, 

// FRAME 05
0x00, 0x60, 0x60, 0xf6, 0x26, 0xff, 0xf5, 0xff, 
0xb3, 0xff, 0x04, 0xbf, 0x04, 0x0e, 0x04, 0x0e, 

// FRAME 06
0x00, 0x00, 0x00, 0x86, 0x06, 0xff, 0xf5, 0xff, 
0xd3, 0xff, 0x64, 0xff, 0x04, 0x6e, 0x04, 0x0e, 

// FRAME 07
0x00, 0x00, 0x00, 0xc6, 0xc6, 0xff, 0x35, 0xff, 
0x53, 0xff, 0x24, 0x7f, 0x04, 0x2e, 0x04, 0x0e,
};

const unsigned char PROGMEM omegaone_plus_mask[] =
{
// width, height,
16, 16,
// FRAME 00
0x60, 0x60, 0xac, 0xec, 0x94, 0xfc, 0x04, 0xfc, 0x04, 0xfc, 0x04, 0xfc, 0x14, 0xfc, 0x1c, 0xfc, 
0x10, 0xf0, 0x20, 0xe0, 0x20, 0xe0, 0x20, 0xe0, 0x20, 0xe0, 0xa0, 0xe0, 0xa0, 0xe0, 0x60, 0x60, 
0x03, 0x03, 0x1a, 0x1b, 0x14, 0x1f, 0x10, 0x1f, 0x10, 0x1f, 0x10, 0x1f, 0x14, 0x1f, 0x1c, 0x1f, 
0x04, 0x07, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x03, 0x03, 

// FRAME 01
0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0xb8, 0xf8, 0x08, 0xf8, 0x08, 0xf8, 0x28, 0xf8, 0x38, 0xf8, 
0x20, 0xe0, 0x20, 0xe0, 0xa0, 0xe0, 0xa0, 0xe0, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0e, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x0a, 0x0f, 0x0e, 0x0f, 
0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

// FRAME 02
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x20, 0xe0, 0x20, 0xe0, 0x60, 0xe0, 
0x40, 0xc0, 0x40, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x02, 0x03, 0x02, 0x03, 0x03, 0x03, 
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM planet_plus_mask[] =
{
// width, height,
32, 32,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe, 0xde, 0xfe, 0x0e, 0xfe, 0x06, 0xfe, 0x12, 0xfe, 
0x00, 0xfe, 0x04, 0xfe, 0x0c, 0xfe, 0x0e, 0xfe, 0x1c, 0xfc, 0x1c, 0xfc, 0x38, 0xf8, 0x38, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xf8, 0xf8, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0x3f, 0xff, 0x08, 0xff, 0x00, 0xff, 0x00, 0xff, 
0x00, 0xff, 0x80, 0xff, 0x80, 0xff, 0xe3, 0xff, 0xfb, 0xff, 0xf8, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e, 0xfe, 0x38, 0xf8, 0xc0, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0x0f, 0x0f, 0x3f, 0x3f, 0x3f, 0xff, 0x3f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3e, 0xff, 0x3c, 0xff, 0x70, 0xff, 0xc0, 0xff, 
0xce, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x07, 0xff, 0x03, 0xff, 0x01, 0xff, 0x01, 0xff, 0xc3, 0xff, 0xc0, 0xff, 0xf0, 0xff, 0x3f, 0x3f, 0x07, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0f, 0x01, 0x0f, 0x03, 0x1f, 0x03, 0x1f, 0x04, 0x3f, 0x04, 0x3f, 0x04, 0x3f, 0x04, 0x3f, 0x09, 0x7f, 0x06, 0x7f, 
0x07, 0x7f, 0x07, 0x3f, 0x0f, 0x3f, 0x27, 0x3f, 0x33, 0x3f, 0x38, 0x3f, 0x1c, 0x1f, 0x1c, 0x1f, 0x0f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//projectiles
const unsigned char PROGMEM bullets_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x3c, 
0x18, 0x3c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 

// FRAME 01
0x00, 0x00, 0x00, 0x18, 0x18, 0x3c, 0x3c, 0x7e, 
0x3c, 0x7e, 0x18, 0x3c, 0x00, 0x18, 0x00, 0x00, 

// FRAME 02
0x00, 0x18, 0x18, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 
0x18, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 0x00, 0x18,
};

const unsigned char PROGMEM items_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x3c, 0x3c, 0x7e, 0x66, 0xff, 0x5a, 0xff, 
0x5a, 0xff, 0x66, 0xff, 0x3c, 0x7e, 0x00, 0x3c, 

// FRAME 01
0xdb, 0xff, 0x83, 0xff, 0x7b, 0xff, 0x70, 0xff, 
0x85, 0xff, 0xdd, 0xff, 0xdd, 0xff, 0xd8, 0xff, 

// FRAME 02
0x00, 0x3e, 0x1c, 0xff, 0xbe, 0xff, 0x00, 0xff, 
0x14, 0xff, 0xbe, 0xff, 0x1c, 0xff, 0x00, 0x3e, 

// FRAME 03
0x18, 0x3c, 0x24, 0x7e, 0x24, 0x7e, 0x18, 0x3c, 
0x08, 0x3c, 0x38, 0x7c, 0x08, 0x3c, 0x38, 0x7c, 

// FRAME 04
0x00, 0x3c, 0x3c, 0x7e, 0x7e, 0xff, 0x76, 0xff, 
0x42, 0xff, 0x7e, 0xff, 0x3c, 0x7e, 0x00, 0x3c, 

// FRAME 05
0x00, 0x3c, 0x3c, 0x7e, 0x7e, 0xff, 0x4a, 0xff, 
0x52, 0xff, 0x7e, 0xff, 0x3c, 0x7e, 0x00, 0x3c, 

// FRAME 06
0x08, 0x3e, 0x22, 0x7f, 0x08, 0x7f, 0x5d, 0xff, 
0x08, 0x7f, 0x22, 0x7f, 0x08, 0x3e, 0x00, 0x08,
};

const unsigned char PROGMEM doors_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x08, 0x1c, 0x0c, 0x1e, 0x0c, 0x1e, 0x0e, 0x1f, 
0x0e, 0x1f, 0x0c, 0x1e, 0x0c, 0x1e, 0x08, 0x1c, 

// FRAME 01
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xff, 0xff, 0x7e, 0xff, 0x18, 0x7e, 0x00, 0x18, 

// FRAME 02
0x10, 0x38, 0x30, 0x78, 0x30, 0x78, 0x70, 0xf8, 
0x70, 0xf8, 0x30, 0x78, 0x30, 0x78, 0x10, 0x38, 

// FRAME 03
0x00, 0x18, 0x18, 0x7e, 0x7e, 0xff, 0xff, 0xff, 
0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM exits_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0xff, 0xff, 0xa5, 0xff, 0xff, 0xff, 0x00, 0xff, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

// FRAME 01
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xa5, 0xff, 0xff, 0xff, 

// FRAME 02
0x07, 0x0f, 0x05, 0x0f, 0x07, 0x0f, 0x05, 0x0f, 
0x05, 0x0f, 0x07, 0x0f, 0x05, 0x0f, 0x07, 0x0f, 

// FRAME 03
0xe0, 0xf0, 0xa0, 0xf0, 0xe0, 0xf0, 0xa0, 0xf0, 
0xa0, 0xf0, 0xe0, 0xf0, 0xa0, 0xf0, 0xe0, 0xf0, 

// FRAME 04
0x00, 0xff, 0xff, 0xff, 0xa5, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xa5, 0xff, 0xff, 0xff, 0x00, 0xff, 

// FRAME 05
0x7e, 0xff, 0x5a, 0xff, 0x7e, 0xff, 0x5a, 0xff, 
0x5a, 0xff, 0x7e, 0xff, 0x5a, 0xff, 0x7e, 0xff,
};

const unsigned char PROGMEM koomer_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 
0x07, 0x0f, 0x03, 0x07, 0x01, 0x03, 0x00, 0x01, 

// FRAME 01
0x00, 0x00, 0x00, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 
0x0f, 0x1f, 0x07, 0x0f, 0x00, 0x07, 0x00, 0x00, 

// FRAME 02
0x00, 0x18, 0x18, 0x3c, 0x0c, 0x1e, 0x06, 0x0f, 
0x06, 0x0f, 0x0c, 0x1e, 0x00, 0x0c, 0x00, 0x00, 

// FRAME 03
0x00, 0x80, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 
0xe0, 0xf0, 0xc0, 0xe0, 0x80, 0xc0, 0x00, 0x80, 

// FRAME 04
0x00, 0x00, 0x00, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 
0xf0, 0xf8, 0xe0, 0xf0, 0x00, 0xe0, 0x00, 0x00,
};

const unsigned char PROGMEM herab_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x10, 0x38, 0x30, 0x78, 0x38, 0x7c, 0x38, 0x7c, 
0x3c, 0x7e, 0x3c, 0x7e, 0x3c, 0x7e, 0x38, 0x7c, 

// FRAME 01
0x38, 0x7c, 0x3c, 0x7e, 0x3c, 0x7e, 0x3c, 0x7e, 
0x38, 0x7c, 0x38, 0x7c, 0x30, 0x78, 0x10, 0x38,
};

const unsigned char PROGMEM grejen_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0xf0, 0xfe, 0x0e, 0xff, 0x08, 0x3e, 0x30, 0x78, 
0x30, 0x78, 0x08, 0x3e, 0x0e, 0xff, 0xf0, 0xfe, 

// FRAME 01
0xe0, 0xf0, 0x10, 0xfe, 0x0e, 0x3f, 0x30, 0x7e, 
0x30, 0x7e, 0x0e, 0x3f, 0x10, 0xfe, 0xe0, 0xf8, 

// FRAME 02
0xc0, 0xe0, 0x20, 0xfc, 0x1c, 0x7e, 0x70, 0xfc, 
0x70, 0xfc, 0x1c, 0x7e, 0x20, 0xfc, 0xc0, 0xf0, 

// FRAME 03
0xf8, 0xff, 0x07, 0xff, 0x04, 0x1f, 0x18, 0x3c, 
0x18, 0x3c, 0x04, 0x1f, 0x07, 0xff, 0xf8, 0xff,
};

const unsigned char PROGMEM krynder_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x18, 0x1c, 0xbe, 0xbc, 0xfe, 0x7a, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xbe, 0xff, 0x1c, 0xfe, 

// FRAME 01
0x1c, 0x7e, 0x6e, 0xff, 0xfe, 0xff, 0xfd, 0xff, 
0xbf, 0xff, 0x7f, 0xff, 0x76, 0xff, 0x38, 0x7e, 

// FRAME 02
0x38, 0x7c, 0x1c, 0x7f, 0x7d, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xbe, 0xff, 0x38, 0xfe, 0x1c, 0x3e, 

// FRAME 03
0x66, 0xff, 0xf3, 0xff, 0xbf, 0xff, 0x3e, 0xff, 
0x7c, 0xff, 0xfd, 0xff, 0xcf, 0xff, 0x66, 0xff, 

// FRAME 04
0x1c, 0xfe, 0xbe, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x7a, 0xff, 0xbc, 0xfe, 0x1c, 0xbe, 0x00, 0x1c, 

// FRAME 05
0x38, 0x7f, 0x76, 0xff, 0x7f, 0xff, 0xbf, 0xff, 
0xfd, 0xff, 0xfe, 0xff, 0x6e, 0xff, 0x1c, 0x7f, 

// FRAME 06
0x1c, 0x3e, 0x38, 0xfe, 0xbe, 0xff, 0xff, 0xff, 
0xff, 0xff, 0x7d, 0xff, 0x1c, 0xff, 0x38, 0x7c, 

// FRAME 07
0x66, 0xff, 0xcf, 0xff, 0xfd, 0xff, 0x7c, 0xff, 
0x3e, 0xff, 0xbf, 0xff, 0xf3, 0xff, 0x66, 0xff,
};

const unsigned char PROGMEM koomerking_plus_mask[] =
{
// width, height,
16, 8,
// FRAME 00
0x00, 0x00, 0x00, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xfe, 0xff, 
0xf6, 0xff, 0xf6, 0xff, 0xfc, 0xfe, 0xf8, 0xfc, 0xf0, 0xf8, 0xe0, 0xf0, 0x00, 0xe0, 0x00, 0x00, 

// FRAME 01
0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfc, 0xfe, 0xfe, 0xff, 0xfe, 0xff, 
0xfe, 0xff, 0xf6, 0xff, 0xf4, 0xfe, 0xfc, 0xfe, 0xf8, 0xfc, 0xf0, 0xf8, 0x00, 0xf0, 0x00, 0x00, 

// FRAME 02
0x00, 0x70, 0x70, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfc, 0xfe, 0xfc, 0xfe, 0xfc, 0xfe, 0xfc, 0xfe, 
0xfc, 0xfe, 0xfc, 0xfe, 0xec, 0xfe, 0xec, 0xfe, 0xfc, 0xfe, 0xf8, 0xfc, 0x70, 0xf8, 0x00, 0x70, 

// FRAME 03
0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfc, 0xfe, 0xfe, 0xff, 0xfe, 0xff, 
0xfe, 0xff, 0xf6, 0xff, 0xf4, 0xfe, 0xfc, 0xfe, 0xf8, 0xfc, 0xf0, 0xf8, 0x00, 0xf0, 0x00, 0x00, 

// FRAME 04
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xfb, 0xff, 0xfb, 0xff, 0xfe, 0xff, 0xfc, 0xfe, 0x78, 0xfc, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char PROGMEM blast_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x3c, 0x7e, 0x42, 0xff, 0x99, 0xff, 0xbd, 0xff, 
0xbd, 0xff, 0x99, 0xff, 0x42, 0xff, 0x3c, 0x7e,
};

const unsigned char PROGMEM karame_plus_mask[] =
{
// width, height,
16, 8,
// FRAME 00
0x3c, 0x7e, 0x7e, 0xff, 0x7e, 0xff, 0x42, 0xff, 0x3c, 0x7e, 0x7e, 0xff, 0x7e, 0xff, 0x42, 0xff, 
0x3c, 0x7e, 0x24, 0x7e, 0x24, 0x7e, 0x7e, 0xff, 0xdb, 0xff, 0xa5, 0xff, 0xa5, 0xff, 0x40, 0xe5, 

// FRAME 01
0x3c, 0x7e, 0x7e, 0xff, 0x7e, 0xff, 0x42, 0xff, 0x3c, 0x7e, 0x7e, 0xff, 0x7e, 0xff, 0x42, 0xff, 
0x3c, 0x7e, 0x24, 0x7e, 0x24, 0x7e, 0x7e, 0xff, 0x5a, 0xff, 0x5a, 0xff, 0x5a, 0xff, 0x24, 0x7e, 

// FRAME 02
0x00, 0xa5, 0xa5, 0xff, 0xa5, 0xff, 0xdb, 0xff, 0x7e, 0xff, 0x24, 0x7e, 0x24, 0x7e, 0x3c, 0x7e, 
0x42, 0xff, 0x7e, 0xff, 0x7e, 0xff, 0x3c, 0x7e, 0x42, 0xff, 0x7e, 0xff, 0x7e, 0xff, 0x3c, 0x7e, 

// FRAME 03
0x00, 0x5a, 0x5a, 0xff, 0x5a, 0xff, 0x5a, 0xff, 0x7e, 0xff, 0x24, 0x7e, 0x24, 0x7e, 0x3c, 0x7e, 
0x42, 0xff, 0x7e, 0xff, 0x7e, 0xff, 0x3c, 0x7e, 0x42, 0xff, 0x7e, 0xff, 0x7e, 0xff, 0x3c, 0x7e,
};

const unsigned char PROGMEM death_plus_mask[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x00, 0x00, 0x24, 0x24, 0x7e, 0x18, 0x3c, 
0x18, 0x3c, 0x24, 0x7e, 0x00, 0x24, 0x00, 0x00, 

// FRAME 01
0x00, 0x42, 0x42, 0xef, 0x08, 0x7e, 0x20, 0x7c, 
0x04, 0x3e, 0x10, 0x7e, 0x42, 0xf7, 0x00, 0x42,
};







/////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////////

int8_t gameState;
#define PlayerWIdth 8
#define PlayerHeight 8
#define GRAVITY 1
#define DIR_LEFT    -1
#define DIR_NONE    0
#define DIR_RIGHT   1
#define PLAYER_DAMAGE_TIME 60
char UNIQUES = 0B00000000;
               //|||||||└-------------------------------> 0 energy tank 1
               //||||||└--------------------------------> 1 energy tank 2
               //|||||└---------------------------------> 2 key
               //||||└----------------------------------> 3 energy blask
               //|||└-----------------------------------> 4 charge beam
               //||└------------------------------------> 5 power suit
               //|└-------------------------------------> 6 koomer king
               //└--------------------------------------> 7 karame

struct Player{
  Point point = {.x = 0, .y = 0};
  int8_t damageTime;
  uint8_t map_id = 0;
  uint8_t prev_map_id = 0;
  int8_t JUMP=0;
  int8_t VSPEED=0;
  int8_t YSPEED=0;
  int8_t JUMPHEIGHT=20;
  int8_t PSPEED = 8;
  int16_t LIFE=100;
  int16_t MAXLIFE=100;
  int8_t POWER=1;
  bool RUN=false;
  bool KEY=false;
  bool POWERSUIT=false;
  bool FACELEFT=false;
  bool FLASH=false;
  int8_t FIRE=0; 
  int8_t frame;
  int8_t cooldown=0;
};

//projectile structure
struct Projectile{
   Rect rect = {.x = 0, .y = 0, .width = 8, .height = 8};
   boolean  Exists=false;
   boolean  dir_left;//Direction the projectiles is moving
   int8_t   PSPEED;
   int8_t   POWER;
};

#define KOOMER_WAITING 0
#define KOOMER_DROPPING 1
#define KOOMER_MOVING 2
#define GREJEN_ON_GROUND 0
#define GREJEN_IN_AIR    1
#define KRYNDER_LOOKING 0
#define KRYNDER_ROLL_LEFT 1
#define KRYNDER_ROLL_RIGHT 2
#define KOOMER_KING_WAITING 0
#define KOOMER_KING_DROPPING 1
#define KOOMER_KING_MOVING 2
#define KOOMER_KING_HOPPING 3

#define KARAME_INITIAL_WAITING 0
#define KARAME_WAITING 1
#define KARAME_CROSSING_ROOM 2
#define KARAME_HEAD_POKES_OUT_TO_SPIT_ACID 3
#define KARAME_SPITS_ACID 4
#define KARAME_HEAD_POKES_OUT_TO_SHAKE_ROOM 5
#define KARAME_SHAKES_ROOM 6
#define KARAME_PULLS_HEAD_BACK 7

/*struct Snowflake{
  Point point = {.x = 0, .y = 0};
  int8_t yspeed = 0;
};*/

struct RainDrop{
  int8_t x=0;
  int8_t y=0;
};

/*
  0:east exit
  1:west exit
  2:north exit
  3:south exit
  4-nirth/east/south/west door
  5=Energy Max Booster
  6=Increased Energy Recharge Rate
  7=key
  8=energy orb
  
  20=koomer/slime
  21=herab
  22=grejen
  23=krynder
  24=Koomer King
  25-karame
*/
struct Entity{    
  public:
   Point point = {.x = 0, .y = 0};
   int8_t    width,height;
   int8_t    life;
   int8_t    pspeed,yspeed=0;//how fast the player moves on the y axis;
   bool      CHARGE=false,Exists=false,FLASH=false;
   int8_t    frame = 0;
   int8_t    Direct=0;//-1,0,1    
   int8_t    id=0;   
   int8_t    Type;
   uint8_t   animation_frames=0;
   uint8_t   animation = 0;  
   uint8_t   action_state = 0;  
};

#define TotalEntities 14
#define TotalMyProjectiles 5
//#define TotalSnowFlakes 20
#define TotalRainDrops 10

Player player;
Projectile myprojectiles[TotalMyProjectiles];
Entity entities[TotalEntities];
//Snowflake snowflakes[TotalSnowFlakes];
RainDrop raindrops[TotalRainDrops];
bool Raining=false;
int8_t karame_heads = 4;





/////////////////////////////////////////////////////////////////////////////////
//Map & Tileset Data
/////////////////////////////////////////////////////////////////////////////////
int16_t WinX=0;
int16_t WinY=0;
#define w 128
#define h 64
#define ScreenWidth 18//viewable tiles in width
#define ScreenHeight 10//viewable tiles in height
int16_t mapDataStart=0;

const uint8_t MapData[2732] PROGMEM = {
  //////////////////////////////////////
  //81 screens in 20 rooms
  /////////////////////////////////////
  //map0
  //71,//length - 1x2
  //16,16,2,4,0,1,84,0,0,21,84,0,0,21,80,0,0,21,80,0,0,21,64,0,0,5,64,0,0,5,64,63,192,1,64,0,0,1,84,0,0,1,84,0,0,1,80,252,0,1,64,0,0,1,64,0,80,0,0,21,85,85,85,90,170,170,85,89,85,85,85,
  199,//length - 2x3
  32,24,2,4,0,1,
  84,0,0,0,0,0,0,21,
  84,0,0,0,0,0,0,5,
  80,0,0,0,0,0,0,1,
  80,0,0,0,0,0,0,1,
  80,0,0,0,0,0,0,1,
  64,0,0,0,0,0,0,0,
  0,0,0,5,85,84,15,245,
  95,3,252,1,85,80,3,245,
  92,0,0,0,5,64,0,5,
  80,0,0,0,0,64,0,5,
  80,0,0,0,0,0,0,5,
  84,0,0,0,0,0,0,5,
  84,0,0,0,0,0,0,5,
  84,0,0,0,0,0,0,5,
  85,0,0,0,0,0,0,5,
  85,0,0,0,0,0,0,21,
  80,0,0,0,0,0,0,21,
  80,0,0,0,0,0,0,5,
  80,0,0,5,85,0,0,5,
  64,0,0,21,85,80,0,1,
  0,21,85,85,85,85,64,1,
  85,90,170,170,170,149,84,0,
  86,169,85,85,85,170,85,85,
  86,85,85,85,85,86,85,85,
  
  //map1
  39,//length - 1X1
  16,8,2,4,0,1,
  85,85,85,85,
  85,85,85,85,
  85,0,21,81,
  80,0,0,65,
  64,0,0,1,
  0,0,0,5,
  85,85,127,85,
  85,85,85,85,
  
  //map2
  231,//length - 1X7
  16,56,2,4,0,1,
  85,85,85,85,
  84,21,64,85,
  80,0,0,21,
  64,0,0,5,
  64,0,0,5,
  0,15,192,1,
  95,0,0,0,
  92,0,0,61,
  
  80,0,0,13,
  80,255,252,1,
  64,0,0,1,
  64,0,0,1,
  0,0,3,245,
  124,0,0,245,
  112,255,192,21,
  64,0,0,85,
  
  64,0,1,85,
  64,0,1,85,
  64,15,193,85,
  64,0,0,85,
  127,0,0,85,
  64,0,0,85,
  64,15,192,85,
  64,0,0,21,
  
  67,192,0,5,
  64,0,0,1,
  80,15,192,0,
  64,0,0,53,
  0,0,3,245,
  124,0,0,61,
  112,15,192,1,
  64,0,0,1,
  
  127,0,3,213,
  92,0,0,21,
  80,15,192,1,
  64,0,0,1,
  127,0,3,245,
  92,0,0,21,
  80,15,192,1,
  64,0,0,1,
  
  95,0,3,245,
  92,0,0,213,
  80,15,192,21,
  80,0,0,21,
  127,0,3,245,
  124,0,0,5,
  64,63,192,0,
  64,0,0,21,
  
  84,0,0,85,
  84,0,5,85,
  80,252,5,85,
  64,0,21,85,
  64,1,85,85,
  0,85,85,85,
  85,85,85,85,
  85,85,85,85,
  
  //map3
  135,//length - 2x2
  32,16,2,4,0,1,
  85,85,85,85,85,85,85,85,
  64,0,0,0,5,84,1,85,
  80,0,85,0,0,80,0,21,
  84,0,21,0,0,16,0,5,
  84,0,7,252,0,0,0,0,
  165,0,0,0,0,0,3,117,
  105,0,0,0,252,15,192,245,
  105,252,0,0,0,0,0,53,
  
  85,0,0,0,1,64,0,21,
  80,0,63,0,5,64,0,21,
  80,0,0,0,21,80,0,5,
  80,0,0,5,85,80,0,5,
  64,0,0,21,85,84,0,1,
  64,21,85,85,85,85,64,5,
  85,90,170,170,170,149,84,21,
  85,89,85,85,85,149,85,85,
  
  //map4
  103,//length - 3x1
  48,8,2,4,0,1,
  85,85,85,85,85,85,85,85,85,85,85,85,
  85,85,0,0,0,5,65,85,84,0,1,85,
  85,64,0,0,0,0,0,85,64,0,0,21,
  80,0,1,85,80,0,0,21,0,0,0,5,
  64,0,5,85,84,0,0,5,0,85,0,1,
  64,1,85,86,148,0,0,1,0,20,0,1,
  0,85,170,170,149,84,0,0,0,0,0,0,
  85,85,149,85,85,85,85,85,85,85,85,85,
  
  //map5
  135,//length - 2x2
  32,16,2,4,0,1,
  100,0,16,0,0,5,85,85,
  100,0,0,0,0,1,65,85,
  100,0,0,0,0,1,0,21,
  100,0,0,0,0,0,0,5,
  100,0,21,84,0,0,0,5,
  101,0,5,85,80,0,1,85,
  101,0,0,85,85,5,85,85,
  165,252,0,0,0,0,85,85,
  
  85,0,0,0,0,0,17,85,
  80,0,63,0,0,0,0,21,
  80,0,0,0,21,80,0,5,
  80,0,0,5,85,80,0,5,
  64,21,64,21,85,84,0,1,
  64,85,85,85,85,85,64,5,
  1,90,170,170,170,149,84,21,
  85,89,85,85,85,149,85,85,
  
  //map6
  199,//length - 3x2
  48,16,2,4,0,1,
  85,85,85,85,85,85,85,85,85,85,85,85,
  85,65,0,1,64,1,85,85,64,0,85,85,
  85,64,0,0,0,0,21,80,0,0,17,85,
  85,0,0,0,0,0,0,64,0,0,0,21,
  85,0,0,0,0,0,0,0,0,0,0,5,
  85,0,0,84,21,64,0,0,0,0,0,0,
  85,64,1,84,5,80,0,0,21,80,255,245,
  69,124,1,80,5,84,0,0,21,64,0,85,
  
  65,124,15,80,1,21,65,84,5,64,0,21,
  64,0,0,64,0,5,0,80,0,64,0,5,
  0,0,0,65,64,0,0,0,0,0,0,1,
  85,67,240,0,0,0,0,0,0,0,0,1,
  85,0,0,0,0,0,0,0,0,0,0,5,
  85,0,0,0,0,0,0,0,85,64,0,5,
  85,255,255,255,255,255,255,213,85,80,1,85,
  170,170,170,170,170,170,170,149,85,85,85,85,
  
  //map7
  135,//length - 4x1
  64,8,2,4,0,1,
  85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,
  84,1,64,80,84,21,80,85,68,5,80,5,85,85,85,85,
  80,0,0,64,0,5,64,1,0,1,64,1,80,84,64,85,
  240,0,0,0,0,4,64,0,0,0,64,0,64,16,0,5,
  0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,1,
  255,240,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  170,95,192,0,0,0,0,0,0,0,0,0,0,85,85,85,
  86,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,
  
  //map8
  135,//length - 2x2
  32,16,2,4,0,1,
  85,85,85,85,85,85,85,85,
  64,0,0,0,5,84,1,85,
  80,0,85,0,0,80,0,21,
  84,0,21,0,0,16,0,5,
  84,0,21,85,0,0,0,5,
  87,240,1,85,80,0,0,245,
  85,192,0,21,85,255,192,53,
  85,192,0,0,85,124,0,53, 
   
  85,192,0,0,5,92,3,213,  
  85,192,0,0,1,84,0,21,
  85,192,0,0,0,87,192,5,
  127,192,0,0,0,84,0,15,
  106,192,0,0,0,0,15,0,
  98,255,255,255,255,255,255,255,
  106,170,170,170,170,170,170,170,
  85,85,85,85,85,85,85,85,
  
  //map9
  71,//length 2x1
  32,8,2,4,0,1,
  85,85,85,85,85,85,85,149,
  68,5,80,0,0,85,85,170,
  64,0,0,0,0,20,64,85,
  64,0,0,3,240,0,0,5,
  127,255,255,0,64,0,0,1,
  84,0,0,1,84,0,0,0,
  0,0,0,5,85,64,1,85,
  85,85,85,85,85,85,85,85,//not made yet
  
  //map10
  167,//length 1x5
  16,40,2,4,0,1,85,85,85,85,170,170,170,170,85,85,85,85,64,65,64,21,64,0,0,5,0,0,0,1,84,0,0,0,85,0,3,213,87,192,0,21,84,0,0,26,80,15,192,25,64,0,0,25,64,0,3,217,124,0,0,25,112,15,192,25,64,0,0,25,127,0,3,217,92,0,0,25,80,15,192,25,64,0,0,25,64,0,3,217,0,0,0,25,124,15,192,25,80,0,0,25,80,0,3,217,80,0,0,217,80,15,192,25,80,0,0,25,95,0,3,217,92,0,0,25,80,63,192,25,80,0,0,25,84,0,0,89,84,0,5,89,80,0,5,89,64,0,21,89,0,1,85,89,85,85,85,89,170,170,170,169,85,85,85,85,
  
  //map11
  135,//not made yet
  32,16,2,4,0,1,85,85,85,85,85,85,85,85,170,170,170,170,170,170,170,169,95,255,255,255,255,255,255,249,124,15,255,252,0,0,0,249,112,3,255,240,0,0,0,57,64,0,255,192,252,0,0,57,0,0,63,0,0,0,0,57,92,0,15,0,0,0,0,57,95,192,15,0,0,0,0,57,172,0,15,255,255,240,0,57,108,0,15,255,255,192,255,249,108,255,0,0,0,0,63,249,108,60,0,0,0,3,255,249,111,255,255,255,255,255,255,249,106,170,170,170,170,170,170,169,85,85,85,85,85,85,85,85,
 
  //map12
  103,//length - 3x1
  48,8,2,4,0,1,
  85,85,85,85,85,85,85,85,85,85,85,85,
  85,84,5,85,85,69,80,5,85,80,16,21,
  84,0,1,85,64,1,64,0,80,64,0,5,
  80,0,0,0,0,0,64,0,64,0,0,1,
  80,1,85,85,85,64,0,0,0,0,0,0,
  64,5,85,170,85,80,0,0,0,0,0,85,
  0,21,85,150,85,85,0,0,0,85,85,85,
  85,85,85,150,85,85,85,85,85,85,85,85,
  
  //map13
  103,//length - 1x3
  16,24,2,4,0,1,
  14,213,85,85,
  14,213,85,85,
  14,255,255,253,
  14,192,0,13,
  14,192,0,13,
  14,192,0,13,
  14,192,0,0,
  14,192,15,213,
  
  14,192,0,213,
  14,255,0,213,
  14,192,0,213,
  14,192,15,213,
  14,192,0,245,
  14,255,0,245,
  14,192,0,21,
  14,192,15,84,
  
  14,192,1,85,
  14,255,1,85,
  14,212,15,85,
  14,212,0,21,
  62,213,64,21,
  62,85,80,5,
  126,85,84,0,
  86,85,85,85,
 
  //map14
  103,//length - 3x1
  48,8,2,4,0,1,
  85,64,0,0,21,85,80,0,0,0,0,85,
  85,0,0,0,1,69,80,5,80,0,0,21,
  84,0,0,80,0,1,64,1,64,0,0,5,
  80,0,0,64,0,0,64,0,64,0,0,1,
  80,0,0,0,0,0,0,0,0,0,0,1,
  64,0,0,0,0,0,0,0,0,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,0,
  85,85,85,85,85,85,85,85,85,85,85,85,
    
  //map15
  135,//length - 2x2
  32,16,2,4,0,1,
  85,85,85,85,85,85,85,85,
  85,85,85,85,64,0,85,85,
  84,0,21,80,0,0,17,85,
  64,0,0,64,0,0,0,21,
  64,0,0,0,0,0,0,5,
  0,0,0,0,0,0,0,5,
  95,245,0,0,21,80,255,245,
  80,5,64,0,17,64,21,85,
  
  64,1,0,84,1,64,1,85,
  64,0,5,80,0,64,1,5,
  64,0,0,0,0,0,0,1,
  64,1,64,0,0,0,0,1,
  80,1,80,0,0,0,0,5,
  85,5,85,85,85,85,85,85,
  85,85,85,85,85,85,85,85,
  85,85,85,85,85,85,85,85,
  
  //map16
  135,//length - 2x2
  32,16,2,4,0,1,
  86,85,85,85,85,86,85,85,
  170,170,170,170,170,170,81,85,
  85,85,85,85,85,85,64,21,
  85,0,0,0,0,16,0,5,
  84,0,0,0,0,0,0,0,
  85,80,63,255,255,255,255,245,
  85,64,0,63,255,255,255,245,
  85,0,0,0,3,255,119,117,
  
  85,252,0,0,0,255,119,85,
  80,0,63,0,0,61,85,85,
  80,0,0,0,0,13,85,85,
  80,255,0,0,0,0,1,21,
  84,60,0,0,0,0,0,21,
  85,85,84,0,0,0,0,85,
  85,85,85,85,85,85,85,89,
  85,85,85,85,85,85,85,85,
  
  //map17
  135,//length - 4x1
  64,8,2,4,0,1,
  85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,
  95,253,64,80,84,20,5,85,68,5,80,5,85,85,85,85,
  94,172,0,64,0,0,1,65,0,1,64,1,80,84,64,85,
  94,236,0,0,0,0,0,0,0,0,64,0,64,16,0,5,
  94,175,252,0,0,0,0,0,0,0,0,0,64,0,0,1,
  95,253,113,15,252,0,250,175,0,0,0,0,0,0,0,0,
  85,85,113,64,0,20,59,236,5,85,0,0,0,85,85,85,
  85,85,85,85,85,85,89,101,85,85,85,85,85,85,85,85,
  
  //map18
  199,//length - 3x2
  48,16,2,4,0,1,85,85,85,85,85,85,85,85,85,85,85,85,85,65,0,1,64,1,85,85,64,0,85,85,85,64,0,0,0,0,21,80,0,0,17,85,85,0,0,0,0,0,0,64,0,0,0,21,85,0,0,0,0,0,0,0,0,0,0,5,85,0,0,127,255,253,64,0,0,5,64,5,85,64,1,124,0,13,80,0,21,85,85,85,255,252,1,80,0,1,80,0,21,85,85,85,255,252,15,80,0,1,85,84,5,85,80,21,240,0,0,64,0,13,85,84,0,69,16,5,0,0,0,65,64,1,85,85,0,1,0,1,255,195,240,0,0,5,85,85,64,0,0,1,85,0,0,0,1,85,85,85,80,0,0,0,85,255,255,255,85,85,85,85,85,85,85,85,170,170,170,170,170,170,170,170,170,170,170,170,85,85,85,85,85,85,85,85,85,85,85,85,
  
  //map19
  135,//length - 2x2
  32,16,2,4,0,1,85,85,85,85,85,85,149,87,170,170,170,170,170,170,191,255,111,255,255,255,255,255,240,0,108,0,0,0,0,0,0,63,108,0,0,0,0,0,0,57,108,0,0,0,0,0,0,57,106,0,0,0,0,0,0,170,108,0,0,0,0,0,0,57,108,0,0,63,252,0,0,57,108,0,0,0,0,0,0,57,106,0,0,0,0,0,0,169,108,0,252,0,0,63,0,57,108,0,0,0,0,0,3,57,111,255,255,255,255,255,255,249,170,170,170,170,170,170,170,170,85,85,85,85,85,85,85,85
};

void findMapStart(int16_t id)
{
  mapDataStart = 0;
  while (id != 0)
  {
    mapDataStart += pgm_read_byte(&MapData[mapDataStart]);
    id--;
  }
  mapDataStart = mapDataStart+1;
}

//type,x,y,dir or special,//
//const int8_t ObjectData[1360] PROGMEM = {//16 entities per map - 20 rooms
const int8_t ObjectData[636] PROGMEM = {
   //2,0,20,1,//door,x,y,dir,
   //3,3,18,1,//energy orb,x,y,dir,
   //4,4,18,1,//wall,x,y,dir,
   //5,5,18,1,//energy tank 1,x,y,dir,
   //6,6,18,1,//energy tank2,x,y,dir,
   //7,7,18,1,//key,x,y,dir,
   //8,8,18,1,//Charge Blast,x,y,dir,
   //9,9,18,1,//Plasma Blast,x,y,dir,
   //10,10,18,1,//power suit,x,y,dir,
   //20,13,4,1,//koomer,x,y,dir,
   //21,12,3,1,//herab,x,y,dir,
   //22,5,12,1,//grenjen,x,y,dir,
   
   //map0
   26,//length
   6,//number of objects
   //7,7,18,1,//key,x,y,dir,
   0,0,6,14,//exit,x,y,map,
   2,0,6,1,//door,x,y,dir,
   0,0,20,12,//exit,x,y,map,
   2,0,20,1,//door,x,y,dir,
   1,31,5,15,//exit,x,y,map,
   1,31,21,2,//exit,x,y,map,
   
   //map1
   10,
   2,//number of objects
   0,0,5,6,//exit,x,y,map,
   5,14,4,1,//energy tank 1,x,y,dir,
   
   //map2
   54,
   13,//number of objects
   0,0,5,0,//exit,x,y,map,
   0,0,12,16,//exit,x,y,map,
   2,0,12,1,//door,x,y,dir,
   0,0,28,3,//exit,x,y,map,
   0,0,53,7,//exit,x,y,map,
   1,15,6,4,//exit,x,y,map,
   1,15,26,6,//exit,x,y,map,
   1,15,46,9,//exit,x,y,map,
   21,7,8,1,//herab,x,y,dir,
   21,6,17,1,//herab,x,y,dir,
   21,8,29,1,//herab,x,y,dir,
   21,11,39,1,//herab,x,y,dir,
   21,3,48,1,//herab,x,y,dir,
   
   //map3
   26,
   6,//number of objects
   1,31,4,2,//exit,x,y,map,
   20,5,1,1,//koomer,x,y,dir,
   20,17,1,1,//koomer,x,y,dir,
   20,22,2,1,//koomer,x,y,dir,
   21,16,5,1,//herab,x,y,dir,
   22,5,12,1,//grenjen,x,y,dir,
   
   //map4
   30,
   7,//number of objects
   0,0,6,2,//exit,x,y,map,
   1,47,6,5,//exit,x,y,map,
   20,9,1,1,//koomer,x,y,dir,
   20,25,1,1,//koomer,x,y,dir,
   20,38,5,1,//koomer,x,y,dir,
   21,39,3,1,//herab,x,y,dir,
   23,35,6,1,//krynder,x,y,dir,
   
   //map5
   34,
   8,//number of objects
   0,0,14,4,//exit,x,y,map,
   20,11,6,1,//koomer,x,y,dir,
   20,19,7,1,//koomer,x,y,dir,
   21,14,3,1,//herab,x,y,dir,
   21,12,8,1,//herab,x,y,dir,
   22,26,5,1,//grenjen,x,y,dir,
   10,30,4,1,//power suit,x,y,dir,
   4,30,4,1,//wall,x,y,dir,
   
   //map6
   42,
   10,//number of objects
   0,0,10,2,//exit,x,y,map,
   1,47,5,1,//exit,x,y,map,
   20,8,1,1,//koomer,x,y,dir,
   20,22,1,1,//koomer,x,y,dir,
   20,29,3,1,//koomer,x,y,dir,
   20,37,1,1,//koomer,x,y,dir,
   21,12,4,1,//herab,x,y,dir,
   21,20,4,1,//herab,x,y,dir,
   21,33,5,1,//herab,x,y,dir,
   23,7,13,1,//krynder,x,y,dir,
   
   //map7
   34,
   8,//number of objects
   0,0,4,8,//exit,x,y,map,
   1,63,5,2,//exit,x,y,map,
   20,6,1,1,//koomer,x,y,dir,
   20,26,1,1,//koomer,x,y,dir,
   20,26,1,1,//koomer,x,y,dir,
   20,39,3,1,//koomer,x,y,dir,
   23,15,6,1,//krynder,x,y,dir,
   23,32,6,1,//krynder,x,y,dir,
   
   //map8
   14,
   3,//number of objects
   1,31,12,7,//exit,x,y,map,
   2,31,12,1,//door,x,y,dir,
   24,5,12,1,//koomer king,x,y,dir,
   
   //map9
   26,
   6,//number of objects
   0,0,6,2,//exit,x,y,map,
   1,31,5,10,//exit,x,y,map,
   20,5,1,1,//koomer,x,y,dir,
   20,10,1,1,//koomer,x,y,dir,
   20,23,2,1,//koomer,x,y,dir,
   23,2,3,1,//krynder,x,y,dir,
   
   //map10
   42,
   10,//number of objects
   0,0,5,9,//exit,x,y,map,
   0,0,21,17,//exit,x,y,map,
   2,0,21,1,//door,x,y,dir,
   0,0,36,18,//exit,x,y,map,
   2,0,36,1,//door,x,y,dir,
   1,15,6,11,//exit,x,y,map,
   21,7,13,1,//herab,x,y,dir,
   21,5,21,1,//herab,x,y,dir,
   21,9,25,1,//herab,x,y,dir,
   21,5,31,1,//herab,x,y,dir,
      
   //map11
   38,
   9,//number of objects
   0,0,6,10,//exit,x,y,map,
   21,5,7,1,//herab,x,y,dir,
   21,9,9,1,//herab,x,y,dir,
   21,12,11,1,//herab,x,y,dir,
   21,13,12,1,//herab,x,y,dir,
   21,20,11,1,//herab,x,y,dir,
   21,26,12,1,//herab,x,y,dir,
   22,16,4,1,//grenjen,x,y,dir,
   8,28,8,1,//Charged Blast,x,y,dir,
   
   //map12
   34,
   8,//number of objects
   0,0,6,13,//exit,x,y,map,
   2,0,6,1,//door,x,y,dir,
   1,47,4,0,//exit,x,y,map,
   2,47,4,1,//door,x,y,dir,
   20,9,1,1,//koomer,x,y,dir,
   20,29,1,1,//koomer,x,y,dir,
   20,40,1,1,//koomer,x,y,dir,
   22,24,6,1,//grenjen,x,y,dir,
   
   //map13
   30,
   7,//number of objects
   1,15,6,14,//exit,x,y,map,
   2,15,6,1,//door,x,y,dir,
   1,15,22,12,//exit,x,y,map,
   2,15,22,1,//door,x,y,dir,
   21,13,5,1,//herab,x,y,dir,
   21,10,10,1,//herab,x,y,dir,
   21,6,14,1,//herab,x,y,dir,
   
   //map14
   30,
   7,//number of objects
   0,0,6,13,//exit,x,y,map,
   2,0,6,1,//door,x,y,dir,
   1,47,6,0,//exit,x,y,map,
   2,47,6,1,//door,x,y,dir,
   20,21,1,1,//koomer,x,y,dir,
   23,12,6,1,//krynder,x,y,dir,
   23,32,6,1,//krynder,x,y,dir,
   
   //map15
   30,
   7,//number of objects
   0,0,5,0,//exit,x,y,map,
   20,5,2,1,//koomer,x,y,dir,
   20,13,3,1,//koomer,x,y,dir,
   20,20,1,1,//koomer,x,y,dir,
   22,3,12,1,//grenjen,x,y,dir,
   22,29,12,1,//grenjen,x,y,dir,
   9,30,5,1,//Plasma Blast,x,y,dir,
   
   //map16
   30,
   7,//number of objects
   1,31,4,2,//exit,x,y,map,
   2,31,4,1,//door,x,y,dir,
   21,8,4,1,//herab,x,y,dir,
   21,6,7,1,//herab,x,y,dir,
   20,15,7,1,//koomer,x,y,dir,
   22,11,13,1,//grenjen,x,y,dir,
   22,27,13,1,//grenjen,x,y,dir,
   
   //map17
   38,
   9,//number of objects
   1,63,5,10,//exit,x,y,map,
   2,63,5,1,//door,x,y,dir,
   20,25,1,1,//koomer,x,y,dir,
   20,45,1,1,//koomer,x,y,dir,
   20,55,2,1,//koomer,x,y,dir,
   21,32,4,1,//herab,x,y,dir,
   22,40,6,1,//grenjen,x,y,dir,
   23,8,3,1,//krynder,x,y,dir,
   6,7,3,1,//energy tank2,x,y,dir,
   
   //map18
   42,
   10,//number of objects
   0,0,10,19,//exit,x,y,map,
   2,0,10,1,//door,x,y,dir,
   1,47,12,10,//exit,x,y,map,
   2,47,12,1,//door,x,y,dir,
   20,5,1,1,//koomer,x,y,dir,
   20,21,1,1,//koomer,x,y,dir,
   20,42,8,1,//koomer,x,y,dir,
   21,25,4,1,//herab,x,y,dir,
   22,12,12,1,//grenjen,x,y,dir,
   23,12,4,1,//krynder,x,y,dir,
   
   //map19 
   26,
   6,//number of objects
   1,31,2,18,//exit,x,y,map,
   2,31,2,1,//door,x,y,dir,   
   25,3,6,1,//karame head,x,y,dir,   
   25,3,10,1,//karame head,x,y,dir,   
   25,27,6,-1,//karame head,x,y,dir,   
   25,27,10,-1,//karame head,x,y,dir,   
};

int16_t findObjectStart(int16_t id)
{
  int16_t i = 0;
  while (id != 0)
  {
    i += pgm_read_byte(&ObjectData[i]);
    id--;
  }
  return i+1;
}

const unsigned char PROGMEM tileset[] =
{
// width, height,
8, 8,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 01
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 02
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 03
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 04
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 05
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 06
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 07
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 08
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 09
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 10
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 11
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 12
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 13
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 14
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 15
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// FRAME 16
0x1e, 0x0f, 0x77, 0x63, 0x9f, 0xcf, 0x47, 0x1e, 
// FRAME 17
0x00, 0x03, 0x03, 0x1b, 0x30, 0x45, 0x3d, 0x0d, 
// FRAME 18
0x1e, 0x0f, 0x77, 0x63, 0x9f, 0xcf, 0xd7, 0xcf, 
// FRAME 19
0x3f, 0x1f, 0x6b, 0x70, 0x85, 0xdd, 0xdd, 0x58, 
// FRAME 20
0xfe, 0xff, 0x07, 0x63, 0x9f, 0x0f, 0xff, 0xfe, 
// FRAME 21
0xff, 0xff, 0x7b, 0x70, 0x85, 0xdd, 0xff, 0xff, 
// FRAME 22
0xfe, 0xff, 0x07, 0x63, 0x9f, 0xcf, 0xd7, 0xcf, 
// FRAME 23
0xff, 0xff, 0x7b, 0x70, 0x85, 0xdd, 0xdd, 0xd8, 
// FRAME 24
0xd7, 0x8f, 0x77, 0x63, 0x9f, 0xcf, 0x47, 0x1e, 
// FRAME 25
0xdb, 0x83, 0x7b, 0x70, 0x85, 0xcd, 0x47, 0x1f, 
// FRAME 26
0xd7, 0x8f, 0x77, 0x63, 0x9f, 0xcf, 0xd7, 0xcf, 
// FRAME 27
0xdb, 0x83, 0x7b, 0x70, 0x85, 0xdd, 0xdd, 0xd8, 
// FRAME 28
0xd7, 0x8f, 0x77, 0x63, 0x9f, 0x0f, 0xff, 0xfe, 
// FRAME 29
0xdb, 0x83, 0x7b, 0x70, 0x85, 0xdd, 0xff, 0xff, 
// FRAME 30
0xd7, 0x8f, 0x77, 0x63, 0x9f, 0xcf, 0xd7, 0xcf, 
// FRAME 31
0xdb, 0x83, 0x7b, 0x70, 0x85, 0xdd, 0xdd, 0xd8, 
// FRAME 32
0x7e, 0xff, 0xc1, 0x83, 0xc5, 0xa3, 0xff, 0x7e, 
// FRAME 33
0x7f, 0xd5, 0xc8, 0xc0, 0xc0, 0xc4, 0xea, 0x7f, 
// FRAME 34
0x7e, 0xff, 0xc1, 0x83, 0xc5, 0xa3, 0xc1, 0x83, 
// FRAME 35
0xff, 0xed, 0xd0, 0xa0, 0xc0, 0x81, 0xc2, 0x83, 
// FRAME 36
0xfe, 0x57, 0x8b, 0x03, 0x03, 0x47, 0xab, 0xfe, 
// FRAME 37
0xff, 0x55, 0x22, 0x00, 0x00, 0x11, 0xaa, 0xff, 
// FRAME 38
0xff, 0xb7, 0x0b, 0x05, 0x03, 0x01, 0x83, 0x81, 
// FRAME 39
0xff, 0x55, 0x22, 0x00, 0x00, 0x01, 0x82, 0x83, 
// FRAME 40
0xc5, 0xa3, 0xc1, 0x83, 0xc5, 0xa3, 0xff, 0x7e, 
// FRAME 41
0x83, 0xc1, 0x82, 0xc0, 0xa0, 0xd0, 0xed, 0xff, 
// FRAME 42
0xc5, 0xa3, 0xc1, 0x83, 0xc5, 0xa3, 0xc1, 0x83, 
// FRAME 43
0xc3, 0xa1, 0xc2, 0x80, 0xc0, 0xa1, 0xc2, 0x83, 
// FRAME 44
0xc3, 0x81, 0x03, 0x03, 0x05, 0x0b, 0x57, 0xff, 
// FRAME 45
0xc3, 0x81, 0x02, 0x00, 0x00, 0x11, 0xaa, 0xff, 
// FRAME 46
0xc5, 0x83, 0x01, 0x03, 0x05, 0x03, 0x81, 0x83, 
// FRAME 47
0xc3, 0x81, 0x02, 0x00, 0x00, 0x01, 0x82, 0x83, 
// FRAME 48
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 49
0x01, 0x03, 0x05, 0x09, 0x09, 0x05, 0x03, 0x01, 
// FRAME 50
0x01, 0x03, 0x07, 0x0b, 0x13, 0x27, 0x43, 0xff, 
// FRAME 51
0x01, 0x03, 0x07, 0x0b, 0x13, 0x27, 0x43, 0xff, 
// FRAME 52
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 53
0xff, 0xc3, 0xa5, 0x89, 0x91, 0xa5, 0xc3, 0xff, 
// FRAME 54
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 55
0xff, 0xc3, 0xa5, 0x89, 0x91, 0xa5, 0xc3, 0xff, 
// FRAME 56
0xff, 0x43, 0x27, 0x13, 0x0b, 0x07, 0x03, 0x01, 
// FRAME 57
0xff, 0x43, 0x27, 0x13, 0x0b, 0x07, 0x03, 0x01, 
// FRAME 58
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 59
0xff, 0xc3, 0xa5, 0x89, 0x91, 0xa5, 0xc3, 0xff, 
// FRAME 60
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 61
0xff, 0xc3, 0xa5, 0x89, 0x91, 0xa5, 0xc3, 0xff, 
// FRAME 62
0xff, 0xc3, 0xa7, 0x8b, 0x93, 0xa7, 0xc3, 0xff, 
// FRAME 63
0xff, 0xc3, 0xa5, 0x89, 0x91, 0xa5, 0xc3, 0xff,
};






//drawNumbersFromRAM - 104 PROGMEM
const unsigned char PROGMEM numbers[] =
{
// width, height,
5, 8,
// FRAME 00
0x0e, 0x19, 0x15, 0x13, 0x0e, 
// FRAME 01
0x00, 0x12, 0x1f, 0x10, 0x00, 
// FRAME 02
0x19, 0x15, 0x15, 0x15, 0x12, 
// FRAME 03
0x11, 0x15, 0x15, 0x15, 0x0a, 
// FRAME 04
0x07, 0x04, 0x04, 0x1e, 0x04, 
// FRAME 05
0x17, 0x15, 0x15, 0x15, 0x09, 
// FRAME 06
0x0e, 0x15, 0x15, 0x15, 0x08, 
// FRAME 07
0x01, 0x01, 0x19, 0x05, 0x03, 
// FRAME 08
0x0a, 0x15, 0x15, 0x15, 0x0a, 
// FRAME 09
0x02, 0x15, 0x15, 0x15, 0x0e,
};
#define ALIGN_RIGHT 2
#define ALIGN_CENTER 1
#define ALIGN_LEFT 0
void drawNumbersRam(int16_t number, byte x, byte y, boolean color, byte alignment)
{
  char buf[5];
  itoa(number, buf, 10);
  char size = strlen(buf);
  for (byte i = 0; i < size; i++)
  {
    char digit = buf[i];
    digit -= 48;
    int16_t alignOffset = 0;
    if(alignment == ALIGN_RIGHT){alignOffset = size*6;}
    else if(alignment == ALIGN_CENTER){alignOffset = size*6>>2;}
    if (color)sprites.drawSelfMasked(x - alignOffset + (6 * i), y, numbers, digit);
    else sprites.drawErase(x - alignOffset + (6 * i), y, numbers, digit);
  }
}


int16_t counter=0;
uint32_t game_timer=0;
bool video_out = false;

void setup() {
  WiFi.mode(WIFI_OFF);
  arduboy.boot(); // raw hardware
  arduboy.blank(); // blank the display
  arduboy.flashlight(); // light the RGB LED and screen if UP button is being held.  
  arduboy.systemButtons();// check for and handle buttons held during start up for system control
  arduboy.audio.begin();
  arduboy.bootLogoSpritesSelfMasked();  
  do {// wait for all buttons to be released
    arduboy.delayShort(50);
  } while (arduboy.buttonsState());
    
  //load sound state
  arduboy.initRandomSeed();  
  gameState  = STATE_MENU_INTRO;
}

const uint16_t SND_WELL_DONE[] PROGMEM = {
  220,100, 0,25, 600,100, 0, 25, 880,500,
  TONES_END 
};

const uint16_t SND_ENTITY_DEATH[] PROGMEM = {
  //kill creature
  120,100,TONES_END
};

const uint16_t SND_OBTAIN_ITEM[] PROGMEM = {
  //obtain item
  220,100,0,25,600,100,0,25,880,500,TONES_END
};

const uint16_t SND_COMPLETE_MISSION[] PROGMEM = {
  //win game
  220,100,0,25,600,100,0,25,880,500,0,300,600,100,0,25,220,100,0,25,600,100,0,25,220,100,0,25,1000,600,TONES_END
};

const uint16_t SND_GAME_OVER[] PROGMEM = {
  //lose game
  150,600,0,200,100,600,0,200,50,1000,TONES_END
};

const uint16_t SND_TITLE_SCREEN[] PROGMEM = {
  //title screen  
  200,200,400,200,200,200,400,200,587,200,150,100,0,100,150,100,0,100,150,100,0,100,
  200,200,400,200,200,200,400,200,784,200,150,100,0,100,150,100,0,100,150,100,0,100,  
  200,200,400,200,200,200,400,200,587,200,150,100,0,100,150,100,0,100,150,100,0,100,
  TONES_END 
};

const uint16_t SND_MISSION_SCREEN[] PROGMEM = {
  2400,200,100,200,2400,200,100,1000,
  2600,200,100,200,2600,200,100,1000,
  2500,200,100,200,2500,200,100,1000,
  0,100,100,1000,TONES_END
};

const char story_text[] PROGMEM = "THE PLANET OF TURNAX\nHAS EXTENSIVE MINERAL\nDEPOSITS OF HIGH\nVALUE TO THE\nINTERPLANETARY ALLIED\nCONFEDERATION (IPAC).\nRECENT UNANTICIPATED\nATTACKS BY PRIMATIVE\nSPECIES HAVE PROVEN\nDAMAGING TO MINING\nOPERATIONS. ";
int16_t x;
int16_t y;
int16_t lspeed;
  
void loop() {
  // put your main code here, to run repeatedly:
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();//polls for buttons down, up and clicked
  char mychar;
  int16_t i;
  if(gameState!=STATE_STORY_SCREEN){
    arduboy.clear();
  }    
  switch(gameState){    
    case STATE_MENU_INTRO:
      sprites.drawSelfMasked(32,0,neoretrologo2,0);
      counter++;
      if(counter>180){
        sound.tones(SND_TITLE_SCREEN);
        counter=0;
        gameState = STATE_TITLE_SCREEN; 
        Raining=true;
        initRainDrops();
      }
      if (arduboy.justPressed(LEFT_BUTTON)) {//activate video out
        Serial.begin(9600);
        video_out=true;
      } 
      break;
    case STATE_TITLE_SCREEN:
      drawRainDrops();
      if (arduboy.justPressed(UP_BUTTON)){
        arduboy.audio.on(); 
        arduboy.audio.saveOnOff();
        sound.tones(SND_TITLE_SCREEN);  
      }//MOVE UP
      if (arduboy.justPressed(DOWN_BUTTON)){
        arduboy.audio.off();
		arduboy.audio.saveOnOff();
      }//MOVE DOWN      
      //show logo image and "PUSH A TO START"
      sprites.drawSelfMasked(44, 0, logo, 0);
      sprites.drawSelfMasked(0, 0, creators, 0);
      //SOUND ON?OFF
      if (arduboy.audio.enabled()) {
        sprites.drawSelfMasked(0,56,sound_icon,0);
      }
      //VERSION NUMBER
      tinyfont.setCursor(113,59);
      tinyfont.print(VERSION);
      if (arduboy.justPressed(A_BUTTON)){
        counter=0;
        x=0;
        y=0;
        lspeed=1;
        arduboy.clear();
        gameState = STATE_STORY_SCREEN; 
      }
      break;
    case STATE_STORY_SCREEN:  
      counter+=lspeed;
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON) || counter>900){  
        if(counter<836){
           lspeed=4;
           counter-=counter%4;
        }else{
          counter=0;     
          sound.noTone();
          game_timer=0;
          gameState = STATE_MISSION_SCREEN;
          sound.tones(SND_MISSION_SCREEN);
        }
      }
      //draw story on the screen 
      i = counter/4;
      if(i>209){i=209;}
//      mychar = pgm_read_byte_near(story_text + i);
      mychar = pgm_read_byte(story_text + i);
      if(counter%4==0 && i<=208){
        if(mychar == '\n'){
          x=-5;
          y+=5;
        }else{
          x+=5;
          if(x>125){
            x=0;
            y+=5;
          }
        }
      }
      
      tinyfont.setCursor(x,y);
      //tinyfont.setCursor((i%25)*5,(i/25)*5);
      tinyfont.print(mychar);
      
      //tinyfont.setCursor(2, 8);
      //tinyfont.print((__FlashStringHelper*)(story_text)); 
      break; 
    case STATE_MISSION_SCREEN:  
      counter++;
      if (arduboy.justPressed(A_BUTTON) || counter>300){
        counter=0;
        gameState=STATE_PLANET_SCREEN;
        player.point.x=0;
        player.point.y=32*8;
      }
      //draw story on the screen 
      tinyfont.setCursor(42, 8);
      tinyfont.print("OBJECTIVE");  
      tinyfont.setCursor(16, 32);
      tinyfont.print("ELIMINATE PRIMITIVE");
      tinyfont.setCursor(42, 37);
      tinyfont.print("HIERARCHY"); 
      tinyfont.setCursor(26, 42);
      tinyfont.print("IN MINIMAL TIME"); 
      break;
    case STATE_PLANET_SCREEN:
      counter++;
      if(counter>300){
        counter=0;
        player.point.x=1088;//17
        player.point.y=1088;//17
        player.damageTime=0;
        player.map_id = 0;
        player.prev_map_id = 0;
        player.JUMP=0;
        player.VSPEED=0;
        player.YSPEED=0;
        player.JUMPHEIGHT=20;
        player.PSPEED = 8;
        player.LIFE=100;
        player.MAXLIFE=100;
        player.POWER=1;
        player.RUN=false;
        player.KEY=false;
        player.POWERSUIT=false;
        player.FACELEFT=false;
        player.FLASH=false;
        player.FIRE=0; 
        player.frame;
        player.cooldown=0;
        UNIQUES=0b11111111;
        karame_heads=4;
        LoadRoom();
        gameState=STATE_GAME_PLAYING;
      }
      sprites.drawPlusMask(48,8,planet_plus_mask,0);
      if((player.point.x>>3)<=16){
        player.point.x+=6;
        player.point.y-=3;
        sprites.drawPlusMask((player.point.x>>3),(player.point.y>>3),omegaone_plus_mask,player.frame);
      }else if((player.point.x>>3)<=32){
        player.frame=1;
        player.point.x+=4;
        player.point.y-=2;
        sprites.drawPlusMask((player.point.x>>3),(player.point.y>>3),omegaone_plus_mask,player.frame);
      }else if((player.point.x>>3)<=58){
        player.frame=2;
        player.point.x+=2;
        player.point.y-=0;
        sprites.drawPlusMask((player.point.x>>3),(player.point.y>>3),omegaone_plus_mask,player.frame);
      } 
      //animation of OmegaOne landing on Planet Turnax
      tinyfont.setCursor(32, 56);
      tinyfont.print("PLANET TURNAX");  
      break;
    case STATE_GAME_PLAYING:
      if(game_timer<4294967295){game_timer++;}
      drawGame();
      break;
    case STATE_GAME_OVER:  
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)){
        counter=0;
        gameState = STATE_TITLE_SCREEN; 
      }    
      tinyfont.setCursor(42, 24);
      tinyfont.print("GAME OVER"); 
      break;      
    case STATE_MISSION_COMPLETE:
      if(counter<300){counter++;}
      if(counter>=300){  
        if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)){
          counter=0;
          gameState = STATE_TITLE_SCREEN; 
        } 
      }
      tinyfont.setCursor(24, 8);
      tinyfont.print("MISSION COMPLETE");  
      tinyfont.setCursor(14, 32);
      tinyfont.print("ELIMINATED PRIMITIVE");
      tinyfont.setCursor(34, 37);
      tinyfont.print("HIERARCHY IN");  
      //add time to complete the mission
      tinyfont.setCursor(26, 42);
      int16_t seconds = (game_timer/60)%60 ;
      int16_t minutes = game_timer/3600;
      tinyfont.setCursor(27, 42);
      tinyfont.print(minutes);
      tinyfont.setCursor(42, 42);
      tinyfont.print("mins");
      tinyfont.setCursor(66, 42);
      tinyfont.print(seconds);
      tinyfont.setCursor(81, 42);
      tinyfont.print("secs");
      break;
  }
  if(video_out){
    Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  }
  arduboy.display();
}

/*
void initSnowFlakes(){
  //create X coord
  for(int a=0;a<TotalSnowFlakes;a++){
    snowflakes[a].point.x = random(WorldWidth(player.map_id));
    snowflakes[a].point.y = random(WorldHeight(player.map_id))<<3;
    snowflakes[a].yspeed = random(3)+2;
  }
}

void drawSnowFlakes(){
  for(int a=0;a<TotalSnowFlakes;a++){
    //move down
    snowflakes[a].point.y+=snowflakes[a].yspeed;
    if((snowflakes[a].point.y>>3)>WorldHeight(player.map_id)){
      snowflakes[a].point.x = random(WorldWidth(player.map_id));
      snowflakes[a].point.y = 0;
      snowflakes[a].yspeed = random(3)+2;
    }
    //maybe move side to side
    int rnd=random(8);
    if(rnd==1){//move left
      snowflakes[a].point.x--;
      if(snowflakes[a].point.x<0){snowflakes[a].point.x++;}
    }
    arduboy.drawPixel(snowflakes[a].point.x-WinX,(snowflakes[a].point.y>>3)-WinY,WHITE);
  }
}*/


void initRainDrops(){
  //create X coord
  for(int16_t a=0;a<TotalRainDrops;a++){
    raindrops[a].x = random(w);
    raindrops[a].y = random(h);
  }
}

void drawRainDrops(){
  for(int16_t a=0;a<TotalRainDrops;a++){
    //move down
    raindrops[a].y+=4;
    if((raindrops[a].y)>h){
      raindrops[a].x = random(w);
      raindrops[a].y = 0;
    }
    //maybe move side to side
    int16_t rnd=random(8);
    if(rnd==1){//move left
      raindrops[a].x--;
      if(raindrops[a].x<0){raindrops[a].x++;}
    }
    arduboy.drawLine(raindrops[a].x,raindrops[a].y,raindrops[a].x,raindrops[a].y+2,WHITE);
  }
}

void drawGame() {
  //drawSnowFlakes();
  if(Raining){
    drawRainDrops();
  }
  
  //lastFrameTime = millis();//time current frame starts
  DrawMap(player.point.x>>3,player.point.y>>3,player.map_id);
  //Serial.print(millis() - lastFrameTime);
  
  playerTick();
  tickProjectiles();
  tickEntities();
  drawProjectiles();
  drawEntities();
  drawPlayer();

  //drawHUD();
  //arduboy.setCursor(0, 16);
  //arduboy.print(Place_Free(player.point.x>>3,(player.point.y>>3)+2));
  //drawWord(1,34,32,WHITE,ALIGN_CENTER);//PLAY
  arduboy.fillRect(0,0,19,7,WHITE);
  arduboy.fillRect(0,0,18,6,BLACK);
  drawNumbersRam(player.LIFE, 0, 0, WHITE, ALIGN_LEFT);
  
  
  //arduboy.setCursor(0,16);
  //arduboy.print(player.map_id);
  //arduboy.setCursor(0,16);
  //arduboy.print(player.point.x>>3);
  //arduboy.setCursor(16,16);
  //arduboy.print(player.point.y>>3);
}








void playerTick(){ 
  if(player.damageTime>0){
    player.damageTime-=1;
  }
  if (arduboy.pressed(LEFT_BUTTON)){ player.RUN=true;player.FACELEFT=true;}
  if (arduboy.pressed(RIGHT_BUTTON)){ player.RUN=true;player.FACELEFT=false;}
  if (arduboy.pressed(A_BUTTON)){ if(player.FIRE==0){player.FIRE=1;}}//shoot
  if (arduboy.justPressed(B_BUTTON)){ 
    if(player.JUMP==0){
      player.JUMP=1;
    }else if(player.JUMP==2 && isEnemyCollision(player.point.x>>3,(player.point.y>>3)+4)){
      player.YSPEED=-player.JUMPHEIGHT;
      if(player.FACELEFT){
        player.frame=3;
      }else{
        player.frame=7;
      }
    } 
  }
  if(!arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(LEFT_BUTTON)){
    player.RUN=false;
    if(player.JUMP==0){
      if(player.FACELEFT){
        player.frame=0;
      }else{
        player.frame=4;
      }
    }
  }
  if( (player.JUMP==1 && (!Place_Free(player.point.x>>3,(player.point.y>>3)+2) || isSolidEntityCollision(player.point.x>>3,(player.point.y>>3)+2) ) ) ){
    player.JUMP=2;
    player.YSPEED=-player.JUMPHEIGHT;
    if(player.FACELEFT){
      player.frame=3;
    }else{
      player.frame=7;
    }
  }
  
  player.VSPEED=player.YSPEED;
  if(player.VSPEED>0){
    do{
      if(Place_Free(player.point.x>>3,(player.point.y+player.VSPEED)>>3) && !isSolidEntityCollision(player.point.x>>3,(player.point.y+player.VSPEED)>>3) ){
        player.point.y+=player.VSPEED;
        player.YSPEED+=GRAVITY;
        if(player.YSPEED>32){player.YSPEED=32;}
          player.VSPEED=0;
        }else{
          if(player.VSPEED>1){
           player.VSPEED--;
          }else{
           player.YSPEED=0;
           player.VSPEED=0;
           player.JUMP=0;
          }
        }
      }while(player.VSPEED>0);
    }else{
      do{
        if(Place_Free(player.point.x>>3,(player.point.y+player.VSPEED)>>3) && !isSolidEntityCollision(player.point.x>>3,(player.point.y+player.VSPEED)>>3) ){
          player.point.y+=player.VSPEED;
          player.YSPEED+=GRAVITY;
          player.VSPEED=0;
        }else{
         if(player.VSPEED<-1){
          player.VSPEED++;
         }else{
           player.YSPEED=0;
           player.VSPEED=0;
           player.JUMP=0;
         }
       }
     }while(player.VSPEED<0);
   }

   if(player.RUN){
    if(player.JUMP!=2){
      if(arduboy.everyXFrames(5)){
        if(player.FACELEFT){
          player.frame+=1;
          if(player.frame>2){player.frame=0;}
        }else{
          player.frame+=1;
          if(player.frame>6){player.frame=4;}
          if(player.frame<4){player.frame=4;}
        }
      }
    }
     if(player.FACELEFT){
       if(Place_Free((player.point.x-player.PSPEED)>>3,player.point.y>>3) && !isSolidEntityCollision((player.point.x-player.PSPEED)>>3,player.point.y>>3) ){
         player.point.x-=player.PSPEED;
       }
     }else{
      if(Place_Free((player.point.x+player.PSPEED)>>3,player.point.y>>3) && !isSolidEntityCollision((player.point.x+player.PSPEED)>>3,player.point.y>>3)){
        player.point.x+=player.PSPEED;
      }  
     }
   }

   //entity collsison
   isEntityCollision();

   //fire weapons  
    player.cooldown--;
    if(player.FIRE==2 && player.cooldown<=0){      
      player.FIRE=0;  
    }
    if(player.FIRE==1){
      player.cooldown=10;
      int16_t newX=0;int16_t newY=0;
      if(!player.FACELEFT){
        newX=(player.point.x>>3);newY=(player.point.y>>3)-1;
      }else{
        newX=(player.point.x>>3);newY=(player.point.y>>3)-1;
      }
      createMyProjectile(newX,newY,player.FACELEFT,player.POWER);
      player.FIRE=2;             
    }
}

bool Place_Free(int16_t TempX, int16_t TempY){
  //set bound box
  Rect rect = {.x = TempX+2, .y = TempY+1, .width = 4, .height = 7};  
  return isPlaceFree(rect,TempX,TempY);
}

bool isPlaceFree(Rect rect, int16_t TX, int16_t TY){
  //verify there are no tiles or objects blocking the location the target wants to move into
  int8_t MX=floor(TX/pgm_read_byte(&tileset[0]));
  int8_t MY=floor(TY/pgm_read_byte(&tileset[1]));
  for(int8_t tempX=MX; tempX<MX+2; tempX++){
    for(int8_t tempY=MY; tempY<MY+4; tempY++){
      int8_t Tile=TileCollision(rect,tempX,tempY,player.map_id);
      if(Tile>=1){return false;}
    }
  }
  return true;
}

void takeDamage(int8_t Dam){
  player.JUMP=2;
  player.YSPEED=-10;
  if(player.damageTime<=0){
    if(player.POWERSUIT){//dam-=1;
      Dam-=1;
    }
    if(player.LIFE>Dam){
      player.FLASH=true;
      player.LIFE-=Dam;
      player.damageTime = PLAYER_DAMAGE_TIME;
    }else{
      sound.tones(SND_GAME_OVER);
      gameState=STATE_GAME_OVER;
    }
  }
}

bool isSolidEntityCollision(int16_t TempX, int16_t TempY){
  Rect player_rect = {.x = TempX+2, .y = TempY+1, .width = 6, .height = 7};  
  //check entities
  for(int8_t i=0;i<TotalEntities;i++){
    if(entities[i].Exists==true){
      Rect entityRect = getEntityRect(i);
      if(arduboy.collide(entityRect,player_rect)){
        if(entities[i].Type==2 && entities[i].FLASH==false){//door 
          return true;
        }else if(entities[i].Type==4){//destructible wall
          return true;
          //player can not pass through if door is visible
        }
      }
    }
  } 
  return false;
}

bool isEnemyCollision(int16_t TempX, int16_t TempY){
  Rect player_rect = {.x = TempX+2, .y = TempY+1, .width = 6, .height = 7};  
  //check entities
  for(int8_t i=0;i<TotalEntities;i++){
    if(entities[i].Exists==true){
      Rect entityRect = getEntityRect(i);
      if(arduboy.collide(entityRect,player_rect)){
        if(entities[i].Type>=20){ 
          return true;
        }
      }
    }
  } 
  return false;
}

  
void isEntityCollision(){
  //set bound box
  Rect rect = {.x=(player.point.x>>3)+2, .y=(player.point.y>>3)+1, .width=6, .height=7};
    
  //check entities
  for(int8_t i=0;i<TotalEntities;i++){
    if(entities[i].Exists==true){
      Rect entityRect = getEntityRect(i);
      if(arduboy.collide(entityRect,rect)){
        if(entities[i].Type<=1){//exits
          //and player is outside of the map
          if(rect.x<0 || rect.y<0 or rect.x+rect.width>WorldWidth(player.map_id) || rect.y+rect.height>WorldHeight(player.map_id)){     
            //set and load the next map 
            //arduboy.setCursor(0,16);
            //arduboy.print(player.map_id);
            //find exit on new map that links to prev map id
            int16_t objStart = findObjectStart(entities[i].id);
            byte num_of_entites_and_objects = pgm_read_byte(&ObjectData[objStart+0]);
            //arduboy.setCursor(96,0);
            //arduboy.print(num_of_entites_and_objects);
            for(byte t=0; t<num_of_entites_and_objects;t++){  
              //for each objects see if it is an exit with next map id = prev map id
              if(pgm_read_byte(&ObjectData[objStart+t*4+1])<=1 && pgm_read_byte(&ObjectData[objStart+t*4+4])==player.map_id){ 
                player.prev_map_id = player.map_id;
                player.map_id = entities[i].id;
                player.point.x=(pgm_read_byte(&ObjectData[objStart+t*4+2])*8)<<3;
                player.point.y=(pgm_read_byte(&ObjectData[objStart+t*4+3])*8)<<3;
                LoadRoom();
                //set camera on player
                WinX = (player.point.x>>3)-68;
                WinY = (player.point.y>>3)-36; 
                arduboy.clear();
              }
            }
          }
        }else if(entities[i].Type==2){//door 
        }else if(entities[i].Type==3){//orb
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          player.LIFE+=5; 
          if(player.LIFE>player.MAXLIFE){player.LIFE=player.MAXLIFE;}
        }else if(entities[i].Type==4){//destructible wall
          //player can not pass through if door is visible
          //entities[i].Exists=false;
          //player.LIFE+=5; 
          //if(player.LIFE>player.MAXLIFE){player.LIFE=player.MAXLIFE;}
        }else if(entities[i].Type==5){//energy tank 1
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,0,0);
          player.LIFE+=100; 
          player.MAXLIFE+=100;
        }else if(entities[i].Type==6){//energy tank 2
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,1,0);
          player.LIFE+=100; 
          player.MAXLIFE+=100;
        }else if(entities[i].Type==7){//key
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,2,0);
          player.KEY=true;
        }else if(entities[i].Type==8){//Energy Blast-unique-dam=3
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,3,0);
          player.POWER=3;
        }else if(entities[i].Type==9){//Plasma Beam-unique-dam=5
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,4,0);
          player.POWER=5;
        }else if(entities[i].Type==10){//Power Suit-25% damage reduction
          sound.tones(SND_OBTAIN_ITEM);  
          entities[i].Exists=false;
          bitWrite(UNIQUES,5,0);
          player.POWERSUIT=true;
        }else if(entities[i].Type==20){//koomer            
          takeDamage(2);
        }else if(entities[i].Type==21){//herab
          takeDamage(3);
        }else if(entities[i].Type==22){//grejen
          takeDamage(4);
        }else if(entities[i].Type==23){//krynder
          takeDamage(5);
        }else if(entities[i].Type==24){//koomer king
          takeDamage(8);
        }else if(entities[i].Type==25){//karame
          takeDamage(0);
        }else if(entities[i].Type==26){//karame energy blast
          takeDamage(5);
        }            
      }
    }
  } 
}

void drawPlayer(){
  if(player.damageTime>0){
    player.FLASH=!player.FLASH;
  }else{
    player.FLASH=false;
  }
  if (!player.FLASH){ 
    sprites.drawPlusMask((player.point.x>>3)-WinX, (player.point.y>>3)-WinY, omega_plus_mask, player.frame);
  }
  //player.FLASH=false;
}










void createMyProjectile(int16_t X, int16_t Y, bool Dir, int8_t power){
  for (int8_t i = 0; i < TotalMyProjectiles; i++){
    if (myprojectiles[i].Exists == false){
      myprojectiles[i].rect.x=X;
      myprojectiles[i].rect.y=Y;
      if(Dir){//go left
        myprojectiles[i].dir_left=true;
      }else{
        myprojectiles[i].dir_left=false;
      }
      myprojectiles[i].Exists=true;
      myprojectiles[i].POWER=power;
      myprojectiles[i].PSPEED=4;
      
      break;
    }
  }
}

void tickProjectiles(){
  for (int8_t i = 0; i < TotalMyProjectiles; i++){
    if (myprojectiles[i].Exists == true){
  //arduboy.setCursor(0, 8*i);
  //arduboy.print(myprojectiles[i].rect.x);
      //move the bullet
      if(myprojectiles[i].dir_left){
          myprojectiles[i].rect.x-=myprojectiles[i].PSPEED;
      }else{
          myprojectiles[i].rect.x+=myprojectiles[i].PSPEED;
      }     
      myprojectileCollision(i);
    }
  }
};

void drawProjectiles(){
  for (int8_t i = 0; i < TotalMyProjectiles; i++){
    if (myprojectiles[i].Exists == true){  
      if(player.POWER==1){
        sprites.drawPlusMask(myprojectiles[i].rect.x-WinX, myprojectiles[i].rect.y-WinY, bullets_plus_mask, 0);        
      }else if(player.POWER==3){
        sprites.drawPlusMask(myprojectiles[i].rect.x-WinX, myprojectiles[i].rect.y-WinY, bullets_plus_mask, 1);        
      }else if(player.POWER==5){
        sprites.drawPlusMask(myprojectiles[i].rect.x-WinX, myprojectiles[i].rect.y-WinY, bullets_plus_mask, 2);        
      }
    }
  } 
};

void myprojectileCollision(int8_t i){
  //set bound box
  Rect rect = {.x=myprojectiles[i].rect.x+2, .y = myprojectiles[i].rect.y+2, 4, 4};  
  //check Enemies
  for(int8_t t=0;t<TotalEntities;t++){
    if(entities[t].Exists){
      Rect entityRect = getEntityRect(t);
      if(arduboy.collide(entityRect,rect)){
        if(entities[t].Type==2){//door
          //if player has the key then the door is destroyed
          if(player.KEY){
            entityTakeDamage(t,myprojectiles[i].POWER);
            myprojectiles[i].Exists=false;
          }
        }else if(entities[t].Type==4){//destructible wall
          if(player.POWER>=3){
            entityTakeDamage(t,myprojectiles[i].POWER);
            myprojectiles[i].Exists=false;
          }
        }else if(entities[t].Type>19 && entities[t].Type<26){//monster
          entityTakeDamage(t,myprojectiles[i].POWER);
          myprojectiles[i].Exists=false;
        }
      }
    }
  }
  //check tiles
  if(!isPlaceFree(rect,myprojectiles[i].rect.x,myprojectiles[i].rect.y)){
    myprojectiles[i].Exists=false;
  }
  //check for boundry
  if(myprojectiles[i].rect.x<0){myprojectiles[i].Exists=false;}
  if(myprojectiles[i].rect.x>WorldWidth(player.map_id)){myprojectiles[i].Exists=false;}  
};






void LoadRoom(){
  //clear all projectiles,entities then initialize the room
  for (int8_t i = 0; i < TotalEntities; i++){
    entities[i].Exists=false;
  }
  for (int8_t i = 0; i < TotalMyProjectiles; i++){
    myprojectiles[i].Exists=false;
  }
  int16_t objStart = findObjectStart(player.map_id);
  byte num_of_entites_and_objects = pgm_read_byte(&ObjectData[objStart+0]);
  for(byte i=0; i<num_of_entites_and_objects;i++){  
    //make sure unique exists if it is - 00001111  health upgrade, weapon upgrade, koomer king, karame
    bool ct=true;
    switch( pgm_read_byte(&ObjectData[objStart+i*4+1]) ){
      case 5://energy tank 1
        ct = (bitRead(UNIQUES,0))==1 ? true : false;
        break;
      case 6://energy tank 2
        ct = (bitRead(UNIQUES,1))==1 ? true : false;
        break;
      case 7://key
        ct = (bitRead(UNIQUES,2))==1 ? true : false;
        break;
      case 8://weapon upgrade 1
        ct = (bitRead(UNIQUES,3))==1 ? true : false;
        break;
      case 9://weapon upgrade 2
        ct = (bitRead(UNIQUES,4))==1 ? true : false;
        break;
      case 10://power suit
        ct = (bitRead(UNIQUES,5))==1 ? true : false;
        break;
      case 24://koomer king
        ct = (bitRead(UNIQUES,6))==1 ? true : false;
        break;
      case 25://Karame
        ct = (bitRead(UNIQUES,7))==1 ? true : false;
        break;
    }
    if(ct){
      createEntity(
        pgm_read_byte(&ObjectData[objStart+i*4+1]),
        pgm_read_byte(&ObjectData[objStart+i*4+2])*8,
        pgm_read_byte(&ObjectData[objStart+i*4+3])*8,
        pgm_read_byte(&ObjectData[objStart+i*4+4])
      ); 
    }
  }
  //initSnowFlakes();
  if(player.map_id==0 || player.map_id==14 || player.map_id==5){
    Raining=true;
    initRainDrops();
  }else{
    Raining=false;
  }
  findMapStart(player.map_id);
}







/*
  0:east exit
  1:west exit
  2:north exit
  3:south exit
  4-nirth/east/south/west door
  5=Energy Max Booster
  6=Increased Energy Recharge Rate
  7=key
  8=energy orb
*/

void createEntity(int8_t Type, int16_t X, int16_t Y, byte special){
  for (int8_t i = 0; i < TotalEntities; i++){
    if (!entities[i].Exists){
      entities[i].id=special;               
      entities[i].point.x=X;
      entities[i].point.y=Y;
      entities[i].Type=Type;
      entities[i].Direct=special;
      entities[i].Exists=true;
      entities[i].FLASH=false;
      entities[i].yspeed=0;
      entities[i].animation=0;
      entities[i].frame=0;
      if(Type==2){//door
        if(entities[i].point.x==0){
          entities[i].frame=1;
        }else{
          entities[i].frame=3;
        }
        entities[i].FLASH=true;
      }else if(Type==20){//slime
        entities[i].life=3;
      }else if(Type==21){//herab
        entities[i].life=5;
      }else if(Type==22){//grejen
        entities[i].life=8;
      }else if(Type==23){//krynder
        entities[i].life=10;
      }else if(Type==24){//koomer king
        entities[i].animation=KOOMER_KING_MOVING;
        entities[i].life=30;
      }else if(Type==25){//karame
        karame_heads=4;
        entities[i].life=100;
      }else if(Type==26){//karame energy blast
        entities[i].life=1;
        //determine path to player
        entities[i].id=player.point.y>>3;
      }
      break;
    }
  }
}

void entityTakeDamage(int8_t i, int8_t damage){
  if(entities[i].Type==23 && entities[i].animation!=KRYNDER_LOOKING){
    return;
  }
  entities[i].life-=damage;
  entities[i].FLASH=true;
  if(entities[i].life<0){
    entities[i].Exists=false;
    if(entities[i].Type==24){//koomer king
      //create the key
      createEntity(7,entities[i].point.x,entities[i].point.y-8,0);
      createEntity(27,entities[i].point.x,entities[i].point.y,0);//death animation
      bitWrite(UNIQUES,6,0);
    }else if(entities[i].Type==25){//must kill all 4 karame heads
      sound.tones(SND_ENTITY_DEATH);
      //game over player wins should game win screen
      createEntity(27,entities[i].point.x,entities[i].point.y,0);//death animation
      karame_heads--;
      if(karame_heads==0){
        bitWrite(UNIQUES,7,0);
        sound.tones(SND_COMPLETE_MISSION);
        gameState=STATE_MISSION_COMPLETE;
      }
    }else if(entities[i].Type>=20){
      sound.tones(SND_ENTITY_DEATH);
      //33% chance to create an energy orb on kill
      if(random(3)==0){
        createEntity(3,entities[i].point.x,entities[i].point.y,0);
      }
      createEntity(27,entities[i].point.x,entities[i].point.y,0);//death animation
    }
  }
}

void entityGravity(int8_t i){
  if(arduboy.everyXFrames(2)){
    int8_t VSPEED = entities[i].yspeed;
    if(VSPEED>0){//falling
      do{
        //verify no tile below me
        if(Entity_Place_Free(i, entities[i].point.x, entities[i].point.y+VSPEED)){
          entities[i].point.y+=VSPEED;
          entities[i].yspeed+=GRAVITY;
          if(entities[i].yspeed>16){entities[i].yspeed=16;}
            VSPEED=0;
          }else{
            if(VSPEED>1){
             VSPEED--;
            }else{
             entities[i].yspeed=0;
             VSPEED=0;
            }
          }
      }while(VSPEED>0);
    }else{
      do{
        //verify no tile above me
        if(Entity_Place_Free(i, entities[i].point.x, entities[i].point.y+VSPEED)){
          entities[i].point.y+=VSPEED;
          entities[i].yspeed+=GRAVITY;
          VSPEED=0;
        }else{
          if(VSPEED<-1){
            VSPEED++;
          }else{//time to start falling
            entities[i].yspeed=0;
            VSPEED=0;
          }
        }
      }while(VSPEED<0);
    }
  }
}

bool Entity_Place_Free(int8_t i, int16_t TempX, int16_t TempY){
  //set bound box
  Rect rect;
  if(entities[i].Type<20){
      rect = {.x=TempX, .y=TempY, .width=8, .height=8};
  }else if(entities[i].Type==20){//koomer
      rect = {.x=TempX+1, .y=TempY+1, .width=6, .height=7};
  }else if(entities[i].Type==21){//herab
      rect = {.x=TempX, .y=TempY+2, .width=8, .height=6};
  }else if(entities[i].Type==22){//grenjen
      rect = {.x=TempX+1, .y=TempY+1, .width=7, .height=7};
  }else if(entities[i].Type==23){//krynder
      rect = {.x=TempX+1, .y=TempY+2, .width=7, .height=6};      
  }else if(entities[i].Type==24){//koomer king
      rect = {.x=TempX+1, .y=TempY+1, 14, 7};
  }else if(entities[i].Type==25){//karame
      rect = {.x=TempX, .y=TempY+1, 16, 6};
  }else if(entities[i].Type==26){//karame energy blast
      rect = {.x=TempX+1, .y=TempY+1, 6, 6};
  }
  return isPlaceFree(rect,TempX,TempY);
}



Rect getEntityRect(byte i){
  if(entities[i].Type<20){
      return {.x=entities[i].point.x, .y=entities[i].point.y, .width=8, .height=8};
  }else if(entities[i].Type==20){//koomer
      return {.x=entities[i].point.x+1, .y=entities[i].point.y+1, .width=6, .height=7};
  }else if(entities[i].Type== 21){//herab
      return {.x=entities[i].point.x, .y=entities[i].point.y+2, .width=8, .height=6};
  }else if(entities[i].Type== 22){//grenjen
      return {.x=entities[i].point.x+1, .y=entities[i].point.y+1, .width=7, .height=7};
  }else if(entities[i].Type== 23){//krynder
      return {.x=entities[i].point.x+1, .y=entities[i].point.y+2, .width=7, .height=6};         
  }else if(entities[i].Type==24){//koomer king
      return {.x=entities[i].point.x+1, .y=entities[i].point.y+1, 14, 7};
  }else if(entities[i].Type==25){//karame
      return {.x=entities[i].point.x, .y=entities[i].point.y+1, 16, 6};
  }else if(entities[i].Type==26){//karame energy blast
      return {.x=entities[i].point.x+1, .y=entities[i].point.y+1, 6, 6};
  }
}

void tickEntities(){
  for (byte i = 0; i < TotalEntities; i++){
    if (entities[i].Exists == true){
      if(entities[i].Type==2){//door        
        if(entities[i].FLASH){
          Rect rect = {.x=(player.point.x>>3), .y=(player.point.y>>3), .width=8, .height=8};
          Rect entityRect = getEntityRect(i);
          //Serial.println((String)rect.x + "-\n");
          //Serial.println((String)rect.y + "-\n");
          //Serial.println((String)entityRect.x + "-\n");
          //Serial.println((String)entityRect.y + "-\n");
          if(!arduboy.collide(entityRect,rect)){
            //Serial.print("closing the door\n");
            entities[i].FLASH=false;
          }
        }
      }else if(entities[i].Type>2 && entities[i].Type<20){//only doors and exits ignore gravity      
        entityGravity(i);  
      }else if(entities[i].Type==20){//koomer
        if(entities[i].animation==KOOMER_WAITING){              
          if(arduboy.everyXFrames(15)){
            entities[i].frame++;
            if(entities[i].frame>1){entities[i].frame=0;}
          }
          //if player is close enough drop on him
          if(entities[i].point.y<player.point.y>>3 && entities[i].point.x<((player.point.x>>3)+8) && entities[i].point.x>((player.point.x>>3)-8)){//see if player is close enough to drop on
            entities[i].animation=KOOMER_DROPPING;
          }
        }else if(entities[i].animation==KOOMER_DROPPING){
          entities[i].frame=2;
          entityGravity(i);
          if(!Entity_Place_Free(i,entities[i].point.x,entities[i].point.y+1)){//else if solid Tile below
            entities[i].animation=KOOMER_MOVING;
            entities[i].Direct=DIR_RIGHT;
          }
        }else if(entities[i].animation==KOOMER_MOVING){
          entityGravity(i);
          if(arduboy.everyXFrames(15)){
            entities[i].frame++;
            if(entities[i].frame>4){entities[i].frame=3;}
          }
          if(arduboy.everyXFrames(2)){
            if(entities[i].point.x<player.point.x>>3){              
              entities[i].Direct=DIR_RIGHT;
              if(Entity_Place_Free(i,entities[i].point.x+1,entities[i].point.y)){
                entities[i].point.x+=1;
              }
            }//move toward player
            if(entities[i].point.x>player.point.x>>3){
              entities[i].Direct=DIR_LEFT;
              if(Entity_Place_Free(i,entities[i].point.x-1,entities[i].point.y)){
                entities[i].point.x-=1;
              }
            }
          }
        }     
      }else if(entities[i].Type==21){//herab
        if( Entity_Place_Free(i,entities[i].point.x+entities[i].Direct,entities[i].point.y) && entities[i].point.x+entities[i].Direct<WorldWidth(player.map_id)-8 && entities[i].point.x+entities[i].Direct>0 ){
          entities[i].point.x+=entities[i].Direct;
        }else{
          if(entities[i].Direct==DIR_LEFT){
            entities[i].Direct=DIR_RIGHT;
            entities[i].frame=0;
          }else{
            entities[i].Direct=DIR_LEFT;
            entities[i].frame=1;
          }
        }
      }else if(entities[i].Type==22){//grejen
        entityGravity(i);
        if(entities[i].animation==GREJEN_ON_GROUND){//on ground run animation
          if(arduboy.everyXFrames(10)){
            entities[i].frame++;
            if(entities[i].frame>3){
              entities[i].frame=0;
              entities[i].animation=GREJEN_IN_AIR;
              if(entities[i].CHARGE){
                entities[i].yspeed=-9;
              }else{
                entities[i].yspeed=-8;
              }
              entities[i].CHARGE=!entities[i].CHARGE;
              //determine direction we are jumping
              if((player.point.x>>3)<entities[i].point.x){//left
                entities[i].Direct=DIR_LEFT;
              }else{
                entities[i].Direct=DIR_RIGHT;
              }
            }
          }
        }else if(entities[i].animation==GREJEN_IN_AIR){//jumping toward player
          //complete jump arc animation until we hit ground, if its a wall we slide down the wall
          //when back on ground start cycle over
          if(!Entity_Place_Free(i, entities[i].point.x, entities[i].point.y+1) && entities[i].yspeed==0){
            entities[i].animation=GREJEN_ON_GROUND;
          }
          //move in Direct chosen
          if(Entity_Place_Free(i, entities[i].point.x+entities[i].Direct, entities[i].point.y)){                
             entities[i].point.x+=entities[i].Direct;
          }
        }
      }else if(entities[i].Type==23){//krynder
        entityGravity(i);
        if(entities[i].animation==KRYNDER_LOOKING){
          //looks around for 2 secs then chooses a direction and rolls 
          if(arduboy.everyXFrames(30)){
            entities[i].frame+=4;
            if(entities[i].frame>4){entities[i].frame=0;}
          }
          if(arduboy.everyXFrames(120)){
            if((player.point.x>>3)<entities[i].point.x){//left
              entities[i].Direct=DIR_LEFT;
              entities[i].animation=KRYNDER_ROLL_LEFT;
            }else{
              entities[i].Direct=DIR_RIGHT;
              entities[i].animation=KRYNDER_ROLL_RIGHT;
            }
          }
        }else if(entities[i].animation==KRYNDER_ROLL_LEFT || entities[i].animation==KRYNDER_ROLL_RIGHT){
          //rolls until he hits a wall then stops and looks around again
          if(arduboy.everyXFrames(3)){
            if(entities[i].animation==KRYNDER_ROLL_LEFT){
              entities[i].frame+=1;
              if(entities[i].frame>3){entities[i].frame=1;}
            }else{
              entities[i].frame+=1;
              if(entities[i].frame>7){entities[i].frame=5;}
            }
          }
          if( Entity_Place_Free(i, entities[i].point.x+entities[i].Direct, entities[i].point.y) && entities[i].point.x+entities[i].Direct<WorldWidth(player.map_id)-8 && entities[i].point.x+entities[i].Direct>0 ){                
            entities[i].point.x+=entities[i].Direct;
          }else{
            entities[i].animation=KRYNDER_LOOKING;
          }
        }    
      }else if(entities[i].Type==24){//koomer king
        if(arduboy.everyXFrames(5)){
          entities[i].frame+=1;
          if(entities[i].frame>3){entities[i].frame=0;}
        }
       if(entities[i].animation==KOOMER_KING_MOVING){
          entityGravity(i);
          if(arduboy.everyXFrames(2)){
            if(entities[i].point.x<player.point.x>>3){              
              if(Entity_Place_Free(i,entities[i].point.x+1,entities[i].point.y)){
                entities[i].point.x+=1;
              }
            }//move toward player
            if(entities[i].point.x>player.point.x>>3){
              if(Entity_Place_Free(i,entities[i].point.x-1,entities[i].point.y)){
                entities[i].point.x-=1;
              }
            }
          }
          if(entities[i].life<25){
            if(arduboy.everyXFrames(60)){
              if(entities[i].point.x<player.point.x>>3){              
                entities[i].animation=KOOMER_KING_HOPPING;
                entities[i].yspeed=-9;
                entities[i].Direct=DIR_RIGHT;
              }//move toward player
              if(entities[i].point.x>player.point.x>>3){      
                entities[i].animation=KOOMER_KING_HOPPING;
                entities[i].yspeed=-9;
                entities[i].Direct=DIR_LEFT;
              }
            }
          }
        }else if(entities[i].animation==KOOMER_KING_HOPPING){
          entityGravity(i);
          if(Entity_Place_Free(i,entities[i].point.x+(entities[i].Direct*2),entities[i].point.y)){
            entities[i].point.x+=(entities[i].Direct*2);
          }
          if(!Entity_Place_Free(i,entities[i].point.x,entities[i].point.y+1)){
            entities[i].animation=KOOMER_KING_MOVING;
          }
        }     
      }else if(entities[i].Type==25){//karame
        //every second all 4 karame heads shoots a energy blast at the player
        if(arduboy.everyXFrames(60)){
          if(entities[i].Direct>0){
            entities[i].frame=0;
            createEntity(26,entities[i].point.x+8,entities[i].point.y,entities[i].Direct);
          }else{
            entities[i].frame=2;
            createEntity(26,entities[i].point.x,entities[i].point.y,entities[i].Direct);
          }
        }else{
          if(entities[i].Direct>0){
            entities[i].frame=1;
          }else{
            entities[i].frame=3; 
          }
        }
      }else if(entities[i].Type==26){//karame energy blast
        //the energy blast moves along its path
        entities[i].point.x+=(entities[i].Direct*2);
        if(entities[i].point.y>entities[i].id){
          entities[i].point.y-=1;
        }else if(entities[i].point.y<entities[i].id){
          entities[i].point.y+=1;
        }
      }else if(entities[i].Type==27){//death animation
        if(arduboy.everyXFrames(15)){
          entities[i].frame++;
          if(entities[i].frame>1){
            entities[i].Exists=false;
          }
        }
      }
      
      //entity dies if it leaves the world
      if(entities[i].point.x>WorldWidth(player.map_id)+8){//right
        entities[i].Exists=false;
      }else if(entities[i].point.x<-8){//left
        entities[i].Exists=false;
      }
    }
  }
};

void drawEntities(){
  for (int8_t i = 0; i < TotalEntities; i++){
    if (entities[i].Exists == true){
      if(entities[i].point.x-WinX>-8 & entities[i].point.x-WinX<w & entities[i].point.y-WinY>-8 & entities[i].point.y-WinY<h & !entities[i].FLASH){
        if(entities[i].Type==0){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, exits_plus_mask, 0);
        }else if(entities[i].Type==1){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, exits_plus_mask, 1);
        }else if(entities[i].Type==2){//door
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, doors_plus_mask, entities[i].frame);
        }else if(entities[i].Type==3){//energy orb
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 0);
        }else if(entities[i].Type==4){//destructable wall
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 1);
        }else if(entities[i].Type==5){//energy tank
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 2);   
        }else if(entities[i].Type==6){//energy tank
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 2);  
        }else if(entities[i].Type==7){//key
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 3);         
        }else if(entities[i].Type==8){//energy blast
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 4);   
        }else if(entities[i].Type==9){//plasma blast
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 5);   
        }else if(entities[i].Type==10){//power suit
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, items_plus_mask, 6);   
        }else if(entities[i].Type==20){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, koomer_plus_mask, entities[i].frame);   
        }else if(entities[i].Type==21){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, herab_plus_mask, entities[i].frame);  
        }else if(entities[i].Type==22){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, grejen_plus_mask, entities[i].frame);  
        }else if(entities[i].Type==23){
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, krynder_plus_mask, entities[i].frame); 
        }else if(entities[i].Type==24){ 
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, koomerking_plus_mask, entities[i].frame); 
        }else if(entities[i].Type==25){//karame
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, karame_plus_mask, entities[i].frame); 
        }else if(entities[i].Type==26){//karame blast ball
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, blast_plus_mask, 0); 
        }else if(entities[i].Type==27){//death animation
          sprites.drawPlusMask(entities[i].point.x-WinX, entities[i].point.y-WinY, death_plus_mask, entities[i].frame); 
        }
      }else if(entities[i].FLASH==true && entities[i].Type>2){
        entities[i].FLASH=false;
      }
    }
  }
};








/////////////////////////////////////////////////////////////////////////////////
//MAP ENGINE
////////////////////////////////////////////////////////////////////////////////
int16_t WorldWidth(int8_t map_id){
  return (pgm_read_byte(&tileset[0]))*(pgm_read_byte(&MapData[mapDataStart+0]));
}

int16_t WorldHeight(int8_t map_id){
  return (pgm_read_byte(&tileset[1]))*(pgm_read_byte(&MapData[mapDataStart+1]));
}

void UpdateCamera(int16_t x, int16_t y, int8_t map_id){
 int16_t WinTargetX= x-68;
 int16_t WinTargetY= y-36; 
    
 WinX+=getCamSmoothMove(WinTargetX-WinX);
 WinY+=getCamSmoothMove(WinTargetY-WinY);   
    
 if(WinX<0){WinX=0;}
 if(WinX>(WorldWidth(map_id)-w)){WinX=(WorldWidth(map_id)-w);}
 if(WinY<0){WinY=0;}
 if(WinY>(WorldHeight(map_id)-h)){WinY=(WorldHeight(map_id)-h);}
}
  
int16_t getCamSmoothMove(int16_t offset) {
 int16_t off = abs(offset);
 if (off >= 128) off = offset < 0 ? -64:64 ;
 else if (off >= 4) off = round(offset / 2);
 else if (off > 0) off = offset < 0 ? -1:1;
 return off;
}

void DrawMap(int16_t x, int16_t y, int8_t map_id){ 
 UpdateCamera(x, y, map_id); 
 int16_t StartX=floor((WinX-(pgm_read_byte(&tileset[0])))/(pgm_read_byte(&tileset[0])));
 int16_t StartY=floor((WinY-(pgm_read_byte(&tileset[1])))/(pgm_read_byte(&tileset[1])));
 if(StartX<0){StartX=0;}
 if(StartY<0){StartY=0;}
 int16_t EndX=StartX+ScreenWidth;
 if(EndX>pgm_read_byte(&MapData[mapDataStart+0])){EndX=pgm_read_byte(&MapData[mapDataStart+0]);}
 int16_t EndY=StartY+ScreenHeight;
 if(EndY>pgm_read_byte(&MapData[mapDataStart+1])){EndY=pgm_read_byte(&MapData[mapDataStart+1]);}
 //lastFrameTime = millis();//time current frame starts
 for(int16_t TempX=StartX;TempX<EndX;TempX++){
  for(int16_t TempY=StartY;TempY<EndY;TempY++){
   int16_t NewTempX=-WinX+(TempX*(pgm_read_byte(&tileset[0])));
   int16_t NewTempY=-WinY+(TempY*(pgm_read_byte(&tileset[1])));
   //if compression is used then we need to do the bit shifting
   int8_t tile = getTile(TempX,TempY, map_id);
   //if(pgm_read_byte(&MapData[i+5])==1){//multitileset
     tile = getMultiTileId(tile,TempX,TempY, map_id);
   //}
   sprites.drawSelfMasked(NewTempX, NewTempY, tileset, tile);
  }
 }
 //Serial.print(millis() - lastFrameTime);
}

int8_t getMultiTileId(int8_t tile, int8_t TempX, int8_t TempY, int8_t map_id){
  //determine which tile to use based on tile start + neighboring tiles
  // get the 4 neighboring tiles
  //determine resulting tile
  int8_t offset = 0;

  if(tile==0 or tile==2){//must match
    if(TempY-1<0){
      offset+=1;
    }else if(getTile(TempX,TempY-1, map_id)==tile){//up tile
      offset+=1;
    }
    if(TempX+1>=pgm_read_byte(&MapData[mapDataStart+0])){
      offset+=2;
    }else if(getTile(TempX+1,TempY, map_id)==tile){//right tile
      offset+=2;
    }
    if(TempY+1>=pgm_read_byte(&MapData[mapDataStart+1])){
      offset+=4;
    }else if(getTile(TempX,TempY+1, map_id)==tile){//down tile
      offset+=4;
    }
    if(TempX-1<0){
      offset+=8;
    }else if(getTile(TempX-1,TempY, map_id)==tile){//left tile
      offset+=8;
    }
  }else{//must not be 0
    if(TempY-1<0){
      offset+=1;
    }else if(getTile(TempX,TempY-1, map_id)>0){//up tile
      offset+=1;
    }
    if(TempX+1>=pgm_read_byte(&MapData[mapDataStart+0])){
      offset+=2;
    }else if(getTile(TempX+1,TempY, map_id)>0){//right tile
      offset+=2;
    }
    if(TempY+1>=pgm_read_byte(&MapData[mapDataStart+1])){
      offset+=4;
    }else if(getTile(TempX,TempY+1, map_id)>0){//down tile
      offset+=4;
    }
    if(TempX-1<0){
      offset+=8;
    }else if(getTile(TempX-1,TempY, map_id)>0){//left tile
      offset+=8;
    }
  }  

  return tile*16+offset;
}

int8_t getTile(uint8_t TempX, uint8_t TempY, uint8_t map_id){
  //if(pgm_read_byte(&MapData[i+3])==4){//4 tiles per byte
     if(TempX%4==0){
       return (pgm_read_byte(&MapData[mapDataStart+TempY*(pgm_read_byte(&MapData[mapDataStart+0])/4)+TempX/4+6]) >> 6 & 0b11);
     }else if(TempX%4==1){
       return (pgm_read_byte(&MapData[mapDataStart+TempY*(pgm_read_byte(&MapData[mapDataStart+0])/4)+TempX/4+6]) >> 4 & 0b11);
     } else if(TempX%4==2){
       return (pgm_read_byte(&MapData[mapDataStart+TempY*(pgm_read_byte(&MapData[mapDataStart+0])/4)+TempX/4+6]) >> 2 & 0b11);
     }else if(TempX%4==3){
       return (pgm_read_byte(&MapData[mapDataStart+TempY*(pgm_read_byte(&MapData[mapDataStart+0])/4)+TempX/4+6]) & 0b11);
     }
  //}
}

int8_t TileCollision(Rect rect, int8_t RX, int8_t RY, int8_t map_id){
 if(RY>=0 && RY<pgm_read_byte(&MapData[mapDataStart+1]) && RX>=0 && RX<pgm_read_byte(&MapData[mapDataStart+0])){//on the map
  Rect tileRect = {.x = (RX*(pgm_read_byte(&tileset[0]))), .y = (RY*(pgm_read_byte(&tileset[1]))), .width = pgm_read_byte(&tileset[0]), .height = pgm_read_byte(&tileset[1])};  
  if(arduboy.collide(rect,tileRect)){   
    return getTile(RX, RY, map_id);
  }
 }
 return 0;
}
