#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "Arduboy2.h"
extern Arduboy2 arduboy;

#define FACE_UP 0
#define FACE_DOWN 1
#define FACE_LEFT 2
#define FACE_RIGHT 3

#define TONE_EXPLORER_FIRE 987
#define TONE_BADGUY_DESTROY 486
#define TONE_SPAWNER_DAMAGE 1200
#define TONE_SPAWNER_DESTROY 300
#define TONE_KEY_ACTIVATE 800

#define TONE_GOLD_GET 600
#define TONE_POO_GET 400
#define TONE_CUP_GET 900
#define TONE_LEMON_GET 1000

#define TREASURE_NONE 0
#define TREASURE_GOLD 1
#define TREASURE_POO 2
#define TREASURE_CUP 3
#define TREASURE_LEMON 4

#define MAP_WIDTH 32
#define MAP_HEIGHT 16

#define GSTR(X) strcpy(progmemString, PSTR(X))

#define FILE_VALID 0
#define FILE_COMPLETION 5

#define FILE_SCORE 0
#define FILE_TIME 2
#define FILE_LEVEL 4
#define FILE_CURRENT_LEVEL 5
#define FILE_HEALTH 6
#define FILE_NAME 6
#define FILE_CONTINUE 9

#define A_DOWN new_a
#define A_PRESSED (new_a && !old_a)
#define A_RELEASED (!new_a && old_a)

#define B_DOWN new_b
#define B_PRESSED (new_b && !old_b)
#define B_RELEASED (!new_b && old_b)

#define UP_DOWN new_up
#define UP_PRESSED (new_up && !old_up)
#define UP_RELEASED (!new_up && old_up)

#define DOWN_DOWN new_down
#define DOWN_PRESSED (new_down && !old_down)
#define DOWN_RELEASED (!new_down && old_down)

#define LEFT_DOWN new_left
#define LEFT_PRESSED (new_left && !old_left)
#define LEFT_RELEASED (!new_left && old_left)

#define RIGHT_DOWN new_right
#define RIGHT_PRESSED (new_right && !old_right)
#define RIGHT_RELEASED (!new_right && old_right)

#define GAME_NOTHING 0
#define GAME_REBOOT 1

#define GAME_SAVE_FILE 6 // Save file for use with the game
#define GAME_RANDOM_FILE 7 // Save file for use with random
#define GAME_GLOVE_OFFSET (GAME_SAVE_FILE * 10 * 5)
#define GAME_RANDOM_OFFSET (GAME_RANDOM_FILE * 10 * 5)

#define GAME_MODE_GLOVE 0
#define GAME_MODE_RANDOM 1

extern int16_t GameSaveOffset;
extern uint8_t GameMode;

extern bool new_a, new_b, new_up, new_left, new_down, new_right;
extern bool old_a, old_b, old_up, old_down, old_left, old_right;

const uint8_t scrw = 128;
const uint8_t scrh = 64;

void updateInput();
void tautInput();


extern char progmemString[52];
// Read PROGMEM string
char* rps(const char* const* str);
char* dps(const char* str);
char* eps(char* str);
char* spsn(int16_t addr, uint8_t len);
int16_t EEPROMReadint16_t(int16_t addr);

void printTime(uint16_t time);

#endif
