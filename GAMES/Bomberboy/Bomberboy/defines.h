#ifndef DEFINES_H
#define DEFINES_H

//#ifdef ESP8266
//#include "../../../Shared.h"
//#define sound (*arduboy_tones)
//#else
#include <Arduboy2.h>
#include <ArduboyTones.h>
extern ArduboyTones sound;
#endif

#define TARGET_FRAMERATE 30
#define VERSION "1.0"

#define MAP_WIDTH_MAX  32
#define MAP_HEIGHT_MAX 16

#define ORIENTATION_MASK  0x03
#define ORIENTATION_LEFT  0x00
#define ORIENTATION_RIGHT 0x01
#define ORIENTATION_UP    0x02
#define ORIENTATION_DOWN  0x03

#define UNIT_FLAG_PASS_BRICK   0x04
#define UNIT_FLAG_ACTIVE       0x08
#define UNIT_FLAG_ALIVE        0x10

#define UNITS_MAX 16
#define BOMBS_MAX 16
#define RADIUS_MAX 24
#define BOMBS_ACTIVATION_FRAMES 150

#define GAME_INFO_ACTIVATION_FRAMES 600

#define STARTUP_CELL_X 1
#define STARTUP_CELL_Y 1

#define LEVEL_TITLE_DURATION_FRAMES 80
#define LEVEL_GAME_OVER_DURATION_FRAMES 180
#define PAUSE_PRESS_DURATION 60
#define INVULNERABILITY 150
#define GHOST_FREEZE_TIME 240


//#endif
