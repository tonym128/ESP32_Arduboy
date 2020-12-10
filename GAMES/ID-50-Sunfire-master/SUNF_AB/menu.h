#ifndef MENU_H
#define MENU_H

#include "globals.h"

byte x_ship;
byte y_ship;
byte flareFrame;


void drawTitleScreen()
{
  x_ship = random(0, 2);
  y_ship = random(0, 2);
  if (arduboy.everyXFrames(4)) flareFrame++;
  for (byte i = 0; i < 16; i++) sprites.drawSelfMasked(i * 8, 0, titleWhite, 0);
  sprites.drawSelfMasked(0, 11, titleSun, 0);
  sprites.drawSelfMasked(58, 11, titleFire, 0);
  sprites.drawSelfMasked(18, 40, titleSolar, 0);

  // left side
  sprites.drawSelfMasked(15, 40, FlareTwo, (((flareFrame % 12) < 8) ?  (flareFrame % 12) : 0));
  sprites.drawSelfMasked(25, 43, FlareFour, ((((flareFrame) % 64) < 16) ?  (flareFrame) % 64 : 0));
  sprites.drawSelfMasked(25, 44, FlareThree, (flareFrame + 4) % 8);
  sprites.drawSelfMasked(32, 46, FlareOne, (((flareFrame % 16) < 8) ?  (flareFrame % 16) : 0));
  sprites.drawSelfMasked(40, 48, FlareTwo, (((flareFrame % 13) < 8) ?  (flareFrame % 13) : 0));

  // middle
  sprites.drawSelfMasked(58, 50, FlareThree, ((((flareFrame + 2) % 8) < 10) ?  ((flareFrame + 2) % 10) : 0));
  sprites.drawSelfMasked(62, 50, FlareOne, ((((flareFrame + 3) % 24) < 8) ?  ((flareFrame + 3) % 24) : 0));
  sprites.drawSelfMasked(68, 50, FlareThree, ((((flareFrame + 5) % 15) < 8) ?  ((flareFrame + 5) % 15) : 0));

  // right side
  sprites.drawSelfMasked(83, 47, FlareTwo, ((((flareFrame + 2) % 15) < 8) ?  ((flareFrame + 2) % 15) : 0));
  sprites.drawSelfMasked(89, 46, FlareThree, (flareFrame) % 8);
  sprites.drawSelfMasked(96, 43, FlareOne, ((((flareFrame + 5) % 16) < 8) ?  ((flareFrame + 5) % 16) : 0));
  sprites.drawSelfMasked(102, 41, FlareTwo, ((((flareFrame) % 11) < 8) ?  ((flareFrame) % 11) : 0));


  sprites.drawPlusMask(42 + x_ship, 33 + y_ship, titleShip_plus_mask, 0);
}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 120)
  {
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  for (byte i = 0; i < 4; i++) sprites.drawErase(13 + (28 * i), 2, titleMenu, i);
  sprites.drawErase(((menuSelection - 2) * 27) + 9, 2, titlePointer, 0);
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2)) menuSelection--;
  if (arduboy.justPressed(B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  drawTitleScreen();
  sprites.drawErase(37, 2, titleInfo, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  for (byte i = 0; i < 3; i++) sprites.drawErase(31 + (28 * i), 2, titleMenu, i + 4);
  sprites.drawErase((arduboy.audio.enabled() * 28) + 54, 2, titlePointer, 0);
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

#endif
