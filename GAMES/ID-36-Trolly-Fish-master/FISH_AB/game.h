#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"

void stateGameInitLevel()
{
  scorePlayer = 0;
  pu_shocks = 0;
  pu_bubbles = 0;
  fr = 60;
  startStarFish();
  initStarFish(0);
  initBonuses();
  powerups = 0x00;   //No starting active powerups
  pu_current = 0;
  pu_timers[PUT_STOP] = 0;        // Timer for PU_STOPFISH
  pu_timers[PUT_PROTECT] = 0;     // Timer for PU_PROTECTFISH
  pu_timers[PUT_SHOCK] = 0;       // Timer for PU_SHOCKFISH
  pu_timers[PUT_MAGNET] = 0;      // Timer for PU_MAGNETFISH
  powerUp.active = false;
  for (byte i = 0; i < MAX_ENEMIES; i++)
  {
    enemyFish[i].xSpeed = -3;
    enemyFish[i].resetPos();
    enemyFish[i].width = 14;
    enemyFish[i].height = 8;
    enemyFish[i].burst = random(10);
    enemyFish[i].burstTimer = random(10, 60);

    enemyFish[i].active = false;
    enemyFish[i].type = 0; //bad fish
  }
  for (byte i = 0; i < 12; i++)
  {
    titleBubbles[i].resetPos(true);
  }

  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
    checkInputs();
    spawnWave();
    updatePowerUp();
    updateStarFish();
    updateBonus();
    updateBubble();
    if (arduboy.everyXFrames(3)) updateEnemies();
    checkIfScored();
    drawTrollyFish();
    drawEnemies();
    drawScore(86, 0, 0);
    drawWeed();
    drawBubbles(true);
    drawHUDPowerUps();

    if (checkGameOver())gameState = STATE_GAME_OVER;
};


void stateGamePause()
{
    sprites.drawSelfMasked(40, 20, pause, 0);
    drawWeed();
    drawBubbles(false);
    if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};


void stateGameOver()
{
    sprites.drawSelfMasked(26, 8, gameOver, 0);
    drawWeed();
    drawScore(32, 36, 1);
    if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
};


#endif
