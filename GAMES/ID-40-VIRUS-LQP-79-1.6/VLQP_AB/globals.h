#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2.h>
#include <ArduboyTones.h>

// constants /////////////////////////////////////////////////////////////////


#define OFFSET_VLQP_START             (EEPROM_STORAGE_SPACE_START + 64)
#define OFFSET_LEVEL                  (OFFSET_VLQP_START + sizeof(int16_t))
#define OFFSET_SCORE                  (OFFSET_LEVEL + sizeof(int))
#define OFFSET_HEALTH                 (OFFSET_SCORE + sizeof(unsigned long))
#define OFFSET_VLQP_END               (OFFSET_HEALTH + sizeof(int16_t))

//define menu states (on main menu)
#define STATE_MENU_INTRO         0
#define STATE_MENU_MAIN          1
#define STATE_MENU_HELP          2
#define STATE_MENU_PLAY          3
#define STATE_MENU_INFO          4
#define STATE_MENU_SOUNDFX       5

//define game states (on main menu)

#define STATE_GAME_PREPARE_LEVEL 6
#define STATE_GAME_NEXT_LEVEL    7
#define STATE_GAME_PLAYING       8
#define STATE_GAME_OVER          9
#define STATE_GAME_PAUSE         10
#define STATE_GAME_END           11

#define STATE_GAME_NEW           12
#define STATE_GAME_CONTINUE      13
#define STATE_GAME_MAYHEM        14


// globals ///////////////////////////////////////////////////////////////////

extern Arduboy2Base arduboy;
extern Sprites sprites;
extern ArduboyTones sound;
extern int16_t gameState;
extern int16_t gameType;
extern unsigned long scorePlayer;
extern int16_t globalCounter;

#endif

// function prototypes ///////////////////////////////////////////////////////

short burp(short start, short goal, uint8_t step);
