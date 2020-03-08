#include "menu.h"
#include "player.h"

// globals ///////////////////////////////////////////////////////////////////

int16_t menuSelection = STATE_MENU_PLAY;
int16_t slideCounter = 0;

// method implementations ////////////////////////////////////////////////////
void drawTitleScreen()
{
  sprites.drawSelfMasked(0, 0, titleScreen00, 0);
  sprites.drawSelfMasked(62, 32, titleScreen01, 0);
  sprites.drawSelfMasked(66, 0, titleScreen02, 0);
}

void drawBadge()
{
  sprites.drawSelfMasked(30, 0, titleScreen02, 0);
  sprites.drawSelfMasked(92, 0, titleScreen03, 0);
}

void setSlidersToZero()
{
  globalCounter = 0;
  slideCounter = 0;
}


void makeItSlide()
{
  slideCounter++;
  if (slideCounter > 22)
  {
    globalCounter++;
    slideCounter = 22;
  }

  if (globalCounter > 5)
  {
    globalCounter = 5;
  }
}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 120)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  for (int16_t i = 0; i < 3; i++)
  {
    sprites.drawOverwrite(127 - slideCounter + (8 * i) , 25, smallMask, 0);
  }
  if (menuSelection == 2) sprites.drawOverwrite(98 , 25, smallMask, 0);
  for (int16_t i = 0; i < 4; i++)
  {
    if (((2 + i) - menuSelection) != 0)
    {
      sprites.drawSelfMasked(128 - slideCounter, 25 + (9 * i), menuText, i);
    }
    if (((2 + i) - menuSelection) == 0) sprites.drawSelfMasked(128 - slideCounter - globalCounter, 25 + (9 * i), menuText, i);
  }
  if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 5))
  {
    menuSelection++;
    globalCounter = 0;
  }
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 2))
  {
    menuSelection--;
    globalCounter = 0;
  }
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    setSlidersToZero();
    gameState = menuSelection;
    menuSelection = STATE_GAME_NEW - 10;
  }
  makeItSlide();
}

void stateMenuHelp()
{
  for (int16_t i = 0; i < 2; i++) sprites.drawSelfMasked(32, 32 * i, qrcode, i);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuPlay()
{

  drawTitleScreen();
  for (int16_t i = 0; i < 3; i++)
  {
    sprites.drawOverwrite(127 - slideCounter + (8 * i) , 25, smallMask, 0);
  }
  if (menuSelection == 2) sprites.drawOverwrite(98 , 25, smallMask, 0);
  for (int16_t i = 0; i < 3; i++)
  {
    if (((2 + i) - menuSelection) != 0)
    {
      sprites.drawSelfMasked(128 - slideCounter, 25 + (9 * i), menuText, i + 7);
    }
    if (((2 + i) - menuSelection) == 0) sprites.drawSelfMasked(128 - slideCounter - globalCounter, 25 + (9 * i), menuText, i + 7);
  }
  if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 4))
  {
    menuSelection++;
    globalCounter = 0;
  }
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 2))
  {
    menuSelection--;
    globalCounter = 0;
  }
  if (arduboy.justPressed(B_BUTTON))
  {
    setSlidersToZero();
    gameState = menuSelection + 10;
    gameType = gameState;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
    setSlidersToZero();
    gameState = STATE_MENU_MAIN;
  }
  makeItSlide();
}


void stateMenuInfo()
{
  drawBadge();

  sprites.drawSelfMasked(48, 33, madeBy00, 0);
  sprites.drawSelfMasked(22, 33, madeBy01, 0);
  sprites.drawSelfMasked(27, 47, madeBy02, 0);


  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  for (int16_t i = 0; i < 3; i++)
  {
    sprites.drawOverwrite(127 - slideCounter + (8 * i) , 25, smallMask, 0);
  }

  if (arduboy.justPressed(DOWN_BUTTON))
  {
    arduboy.audio.on();
    globalCounter = 0;
  }
  if (arduboy.justPressed(UP_BUTTON))
  {
    arduboy.audio.off();
    globalCounter = 0;
  }
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    setSlidersToZero();
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }

  sprites.drawSelfMasked(128 - slideCounter, 25 , menuText, 4);
  if (arduboy.audio.enabled())
  {
    sprites.drawSelfMasked(128 - slideCounter, 34, menuText, 5);
    sprites.drawSelfMasked(128 - slideCounter - globalCounter, 43, menuText, 6);
  }
  else
  {
    sprites.drawSelfMasked(128 - slideCounter, 43, menuText, 6);
    sprites.drawSelfMasked(128 - slideCounter - globalCounter, 34, menuText, 5);
  }
  makeItSlide();
}
