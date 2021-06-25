#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 180)
  {
    globalCounter = 0;
    for (byte i = 0; i < 16; i++)
    {
      titleBubbles[i].resetPos(false);
    }
    gameState = STATE_MENU_MAIN;
  }
};

void stateMenuMain()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, titleScreen, i);
  for (byte k = 0; k < 2; k++)
  {
    for (byte j = 0; j < 2; j++)
    {
      sprites.drawPlusMask(73 + (j * 26), 2 + (k * 12), titleMenu_plus_mask, j + (2 * k));
    }
  }
  sprites.drawPlusMask(73 + (menuX * 26), 2 + (menuY * 12), bubbles_plus_mask, bubblesFrame);
  if (arduboy.justPressed(RIGHT_BUTTON) && (!menuX)) menuX = !menuX;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuX)) menuX = !menuX;
  if (arduboy.justPressed(DOWN_BUTTON) && (!menuY)) menuY = !menuY;
  if (arduboy.justPressed(UP_BUTTON) && (menuY)) menuY = !menuY;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = 2 + menuX + (2 * menuY);
}

void stateMenuHelp()
{
  sprites.drawSelfMasked(32, 0, qrcode, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuInfo()
{
  sprites.drawSelfMasked(14 , 8 , trollyFishTitle, 0);
  sprites.drawSelfMasked(26 , 30 , madeBy01, 0);
  sprites.drawSelfMasked(51 , 38 , madeBy02, 0);
  sprites.drawSelfMasked(30 , 50 , madeBy03, 0);
  drawWeed();
  drawBubbles(false);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, titleScreen, i);
  sprites.drawPlusMask(88  , 2 , soundMenu_plus_mask, 0);
  for (byte j = 0; j < 2; j++)
  {
    sprites.drawPlusMask(78 + (24 * j) , 14 , soundMenu_plus_mask, 1 + j);
  }
  sprites.drawPlusMask(76 + (arduboy.audio.enabled() * 22) , 14 , bubbles_plus_mask, bubblesFrame);
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


void stateMenuPlay()
{
  gameState = STATE_GAME_INIT_LEVEL;
}


#endif
