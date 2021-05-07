#define SPAWN_RATE                   0
#define MAX_EASY_ON_STAGE            1
#define MAX_EASY_ENEMY_TYPE          2
#define MAX_MEDIUM_ON_STAGE          3
#define MAX_MEDIUM_ENEMY_TYPE        4
#define MAX_FORMATION_ELEMENT        5
#define SHOOT_SPAWN_RATE             6
#define SHOOT_NUMBER                 7
#define LEVEL_TIMER                  8
#define BOSS_TYPE                    9
#define BOSS_SHOOTS_SPAWN_RATE       10
#define BOSS_SHOOTS                  11

PROGMEM const unsigned char Levels[8][12] = {
  {16,  6,   0,  0,  0,  3,  26,  4,  40,  0,  0,   0},
  {14,  7,   1,  0,  0,  3,  25,  5,  40,  0,  0,   0},
  {12,  8,   2,  0,  0,  4,  24,  6,  45,  0,  0,   0},
  {11,  10,  3,  0,  0,  4,  23,  7,  45,  1,  12,  8},
  {10,  6,   1,  1,  0,  5,  22,  8,  50,  0,  0,   0},
  {10,  8,   2,  2,  1,  5,  21,  8,  45,  2,  10,  8},
  {10,  9,   3,  3,  1,  6,  20,  8,  60,  0,  0,   0},
  {10,  10,  3,  4,  1,  6,  19,  8,  70,  3,  10,  10},
};

const char string_1[] PROGMEM = "1 counterattack";
const char string_2[] PROGMEM = "2 they keep coming";
const char string_3[] PROGMEM = "3 harakiri";
const char string_4[] PROGMEM = "4 they are everywhere";
const char string_5[] PROGMEM = "5 getting bigger";
const char string_6[] PROGMEM = "6 keep shooting";
const char string_7[] PROGMEM = "7 oh oh";
const char string_8[] PROGMEM = "8 the final countdown";

const char* const string_table[] PROGMEM = {string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8};

void levelintro(byte level)
{
  drawstring((const char*)pgm_read_ptr(&(string_table[level])), 2, 8);
}

byte readleveldata(byte level, byte type)
{
  return pgm_read_byte(&Levels[level][type]);
}
