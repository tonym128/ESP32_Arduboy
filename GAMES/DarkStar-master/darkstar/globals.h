#pragma once
#include "gfx/blinkyicon.h"
#include "gfx/blinkyinfo.h"
#include "gfx/darkstarSprite.h"
#include "gfx/darkSprite.h"
#include "gfx/starSprite.h"
#include "gfx/tiles.h"
#include "gfx/hudNumbers.h"
#include "gfx/ballSprite.h"
#include "gfx/joystickSprite.h"
#include "gfx/blockerSprite.h"
#include "levels.h"
#include "print.h"

#define levelWidth  16
#define levelHeight 9

#define frameRate 60
#define gsTitle 0
#define gsTitleEnd 1
#define gsPlayLevel 2
#define gsLevelCompleted 3
#define gsNextLevel        4
#define gsMissionCompleted 5
#define gsGameOver         6
#define gsShowMenu         7

#define introTitleDelay     80
#define introButtonOnDelay  (introTitleDelay + 40)
#define introButtonOffDelay (introButtonOnDelay + 24)
#define introLoopDelay      (introButtonOnDelay - 24)

#define lsIdle          0
#define lsMoving        1
#define lsSwitchControl 2
#define lsCompleted     3

#define ttSpace        0
#define ttWall         1
#define ttCollectible  2

#define maxMoves          1024
#define maxRetries        5
#define nextLevelPenalty  100


struct Camera
{
  int8_t x;
  int8_t y;
};

struct  PlayerSprite
{
  uint8_t pos;
  uint8_t row;
  uint8_t col;
  int8_t  x;
  int8_t  y;
  int8_t  h;
  int8_t  v;
  uint8_t count;
  uint8_t frame;
};

struct  ControlSprite 
{
  int16_t  x;
  int16_t  y;
  int16_t  h;
  int16_t  v;
  uint8_t count;
  uint8_t frame;
};

Arduboy2Base arduboy;

uint8_t levelMap [levelWidth * levelHeight];

Camera camera = {0,-4};

uint8_t gameState;
uint16_t gameMoves;
uint16_t gameOption;

uint8_t levelRetries;
uint8_t levelNumber;
uint8_t levelMoves;
uint8_t levelCollectibles;
uint8_t levelState;
uint8_t levelRetry;

bool gameNextState;

ControlSprite control;
PlayerSprite  ball;
PlayerSprite  blocker;
PlayerSprite* player;


uint8_t sp = 8 + 5;

 uint8_t  anyButtonText[] = 
{
  AT_X((WIDTH - 9 * fontWidth - 5) / 2), AT_Y(HEIGHT - fontHeight + 1),
  'A','N','Y',CHR_SPACE,'B','U','T','T','O','N',
  0
};
 uint8_t  levelCompletedText[] = 
{
  AT_X(32), AT_Y(6),
  'L','E','V','E','L',
  AT_X(26), AT_Y(17),
  'C','O','M','P','L','E','T','E','D',
  AT_X(16), AT_Y(28),
  'I','N',
  AT_X(67), AT_Y(28),
  'M','O','V','E','S',
  AT_X(43), AT_Y(39),
  'M','O','V','E','S',CHR_SPACE,'L','E','F','T',
  AT_X(WIDTH - 5 * fontWidth - 1), AT_Y(HEIGHT - fontHeight - 3),
  CHR_BTN_B,'N','E','X','T',
  0
};

 uint8_t  retryLevelText[] = 
{
  AT_X(2), AT_Y(HEIGHT - fontHeight - 3),
  CHR_BTN_A,'R','E','T','R','Y',
  0
};

 uint8_t  missionCompletedText[] = 
{
  AT_X(1), AT_Y(6),
  'C','O','N','G','R','A','T','U','L','A','T','I','O','N','S',
  AT_X(30), AT_Y(17),
  'Y','O','U',CHR_SPACE,'H','A','V','E',
  AT_X(25), AT_Y(28),
  'C','O','M','P','L','E','T','E','D',
  AT_X(16), AT_Y(39),
  'Y','O','U','R',CHR_SPACE,'M','I','S','S','I','O','N',
  AT_X(13), AT_Y(50),
  'I','N',
  AT_X(71), AT_Y(50),
  'M','O','V','E','S',
  0
};

 uint8_t  menuPos[4][2] =
{
  {21, 10},
  {32, 21},
  {36, 32},
  {38, 43},
};

 uint8_t  menuText[] = 
{
  AT_X(31), AT_Y(11),
  'C','O','N','T','I','N','U','E',
  AT_X(42), AT_Y(22),
  'R','E','T','R','Y',
  AT_X(46), AT_Y(33),
  'N','E','X','T',
  AT_X(48), AT_Y(44),
  'E','X','I','T',
  0
};

 uint8_t  shortMenuPos[2][2] =
{
  {21, 21},
  {38, 33},
};

 uint8_t  shortMenuText[] = 
{
  AT_X(31), AT_Y(22),
  'C','O','N','T','I','N','U','E',
  AT_X(48), AT_Y(33),
  'E','X','I','T',
  0
};

 uint8_t  gameOverText[] = 
{
  AT_X(5), AT_Y(6),
  'O','O','P','S',CHR_EX,CHR_SPACE,'Y','O','U',CHR_SPACE,'H','A','V','E',
  AT_X(5), AT_Y(17),
  'N','O',CHR_SPACE,'M','O','V','E','S',CHR_SPACE,'L','E','F','T',CHR_EX,
  AT_X(25), AT_Y(39),
  'G','A','M','E',CHR_SPACE,'O','V','E','R',
  0
};
