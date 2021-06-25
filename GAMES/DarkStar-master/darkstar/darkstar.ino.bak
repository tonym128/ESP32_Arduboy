// Dark Star 1.0 for Arduboy by Mr.Blinky Dec.2019 - Feb.2020

// A little puzzle game based on the HP48 / TI83 calculator game Dstar.
// Original levels 1-20 by by HpDUSS, levels 21-25 Joe Wingbermuehle.

// Move the ball with the directional buttons and collect all bubbles.
// The ball will continue moving into the same direction until it hits
// a wall or the blocker.

// The blocker can be moved in same way as the ball except it also stops
// when at hits the ball or a bubble.

// Press the B-button to switch control between the ball and the blocker.
// A joystick will wiggle its way from the ball to the blocker or vice
// versa when changing control.

// Each time you move the ball or blocker will count as a move. You have 
// 1024 moves to complete all 25 levels.

// As an aid you get 5 special balls. A special ball can be used to either
// retry or skip a level. Skipping a level takes a 100 move penalty.

// Press the A button for the in game option menu.

// TO DO:
// add sound effects
// add 2nd mission with more levels

#include "game.h"

void setup()
{
  arduboy.boot(); 
  arduboy.flashlight();
  arduboy.systemButtons();
  //arduboy.audio.begin();
  //arduboy.bootLogo();
  //arduboy.waitNoButtons();
  for (uint8_t x = WIDTH; x >= ((WIDTH - blinkyiconWidth) / 2) ; x -= 2)
  {
    while (!arduboy.nextFrame());
    Sprites::drawSelfMasked(x, 0 , blinkyicon,0);
    if (x == (WIDTH - blinkyiconWidth) / 2) Sprites::drawSelfMasked((WIDTH - blinkyinfoWidth) / 2 + 1, HEIGHT - blinkyinfoHeight + 2, blinkyinfo, 0);
    arduboy.display(CLEAR_BUFFER);
  }
  arduboy.delayShort(1500);
  //arduboy.resetGame();
}

void loop()
{ 
  delay(0);
  if(!arduboy.nextFrame())
    return;
  arduboy.pollButtons();

  switch (gameState)
  {
    case gsTitle:
      title();
      break;

    case gsPlayLevel:
      playLevel();
      drawLevel();
      break;

    case gsLevelCompleted:
      if (levelNumber < numberOfLevels)
        levelCompleted();
      else
        missionCompleted();
      break;

    case gsGameOver:
      gameOver();
      break;
      
    case gsShowMenu:
      showMenu();
      break;
  }

  arduboy.display(CLEAR_BUFFER);
}
