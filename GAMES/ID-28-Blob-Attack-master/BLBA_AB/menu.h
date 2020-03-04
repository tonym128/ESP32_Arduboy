#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

byte sequenceFrame;
const unsigned char PROGMEM frameSequences[] = {
  // bouncing Ball Frame Sequence
  0, 1, 2, 0, 0, 0, 0, 0,
  // bouncing Ball Height Sequence
  0, 0, 0, 1, 0, 0, 0, 0,
  // game title Height Sequence
  1, 2, 3, 4, 5, 4, 3, 2,
};

void drawTitleScreen()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(0, 0, splashScreen, 0);
}

void stateMenuIntro()
{
  if (globalCounter < 120)
  {
    globalCounter++;
    sprites.drawSelfMasked(34, 4, T_arg, 0);
  }
  else
  {
    gameState = STATE_MENU_MAIN;
    globalCounter = 0;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  sprites.drawPlusMask(17 + (24 * (menuSelection - 2)), 52, selectorOne_plus_mask, 0);
  sprites.drawErase(19, 54, mainMenu, 0);
  sprites.drawPlusMask(25 + (24 * (menuSelection - 2)), 44 - pgm_read_byte(&frameSequences[sequenceFrame + 8]), bouncingBall_plus_mask, pgm_read_byte(&frameSequences[sequenceFrame]));
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5))menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2))menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  sprites.drawSelfMasked(32, 0, qrcode, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(19, 27, madeBy, 0);
  sprites.drawSelfMasked(5, pgm_read_byte(&frameSequences[sequenceFrame + 16]), gameTitle, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  sprites.drawPlusMask(61 + (19 * arduboy.audio.enabled()), 52, selectorTwo_plus_mask, 0);
  sprites.drawErase(31, 54, soundMenu, 0);
  sprites.drawPlusMask(67 + (19 * arduboy.audio.enabled()), 44 - pgm_read_byte(&frameSequences[sequenceFrame + 8]), bouncingBall_plus_mask, pgm_read_byte(&frameSequences[sequenceFrame]));
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

#endif
