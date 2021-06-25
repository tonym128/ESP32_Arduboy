#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "globals.h"

// Powerup Timers
#define PUT_STOP                   0
#define PUT_PROTECT                1
#define PUT_SHOCK                  2
#define PUT_MAGNET                 3

// Powerups
#define PU_SHOOTFISH               0
#define PU_TURNFISH                1
#define PU_MAGNETFISH              2
#define PU_POPFISH                 3
#define PU_PROTECTFISH             4
#define PU_LIFEFISH                5
#define PU_SHOCKFISH               6
#define PU_STOPFISH                7

#define SHOCK_LENGTH               8

extern byte pu_timers[];
extern byte powerups;

byte shock_burst = 0;
byte trollyFrame = 0;
struct Player
{
  public:
    int y;
    byte width, height;
    byte blink;
};

Player trollyFish = {.y = 32, .width = 2, .height = 4, .blink = false };

void drawTrollyFish()
{
  if (arduboy.everyXFrames(3) && shock_burst > 0) // Manage shock bursting
    {
      sound.tone(900, 25);
      --shock_burst;
    }

  if (arduboy.everyXFrames(5) && trollyFish.blink > 0)
    --trollyFish.blink;
    
  byte faster = 1;
  if (arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) faster = 2;
  
  if (arduboy.everyXFrames(6 / faster)) trollyFrame++;

  if (trollyFrame > 3 ) trollyFrame = 0;
  if (trollyFish.blink == 0 || trollyFish.blink % 4)
  sprites.drawPlusMask(14, trollyFish.y - 8, Trolly_plus_mask, trollyFrame);
  
  // Protect Powerup
  if (bitRead(powerups,PU_PROTECTFISH) && (pu_timers[PUT_PROTECT] > 60 || pu_timers[PUT_PROTECT] % 2 == 0))
    sprites.drawPlusMask(10, trollyFish.y - 8 -4, bigBubble_plus_mask, 0);

  // Shock Powerup
  if (shock_burst > 0)
  {
    sprites.drawSelfMasked(10, trollyFish.y - 8 -4, shockAura, trollyFrame);    
  }
  
}

#endif
