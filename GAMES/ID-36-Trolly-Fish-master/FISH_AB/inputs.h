#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "globals.h"
#include "powerups.h"

#define GAME_TOP                8
#define GAME_BOTTOM             60
#define GAME_LEFT               3
#define GAME_RIGHT              112


void checkInputs()
{
  if (arduboy.pressed(UP_BUTTON) && (trollyFish.y > GAME_TOP)) trollyFish.y--;
  if (arduboy.pressed(DOWN_BUTTON) && (trollyFish.y < GAME_BOTTOM)) trollyFish.y++;
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PAUSE;
  if (arduboy.justPressed(B_BUTTON))
  {
    if (pu_shocks > 0 && shock_burst == 0)
    {
      --pu_shocks;
      shock_burst = SHOCK_LENGTH;
    }
    if (pu_bubbles > 0) shootBubble();
  }
}


#endif
