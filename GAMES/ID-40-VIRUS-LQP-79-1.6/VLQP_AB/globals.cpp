#include "globals.h"

// globals ///////////////////////////////////////////////////////////////////

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);
unsigned long scorePlayer;
int16_t gameID;
int16_t gameState  = STATE_MENU_INTRO;
int16_t gameType = STATE_GAME_NEW;
int16_t globalCounter = 0;

// function implementations //////////////////////////////////////////////////

// burp
// returns the value a given percent distance between start and goal
// percent is given in 4.4 fixed point
short burp(short start, short goal, uint8_t step)
{
  short a = goal;
  short b = start;
  int8_t sign = 0;

  if (start > goal)
  {
    a = start;
    b = goal;
    sign = -1;
  }
  else if (start < goal)
  {
    sign = 1;
  }

  start += sign * (1 + ((a - b) * step) / 16);
  if (a < b) return goal;

  return start;
}
