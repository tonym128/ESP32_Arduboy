#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "playfield.h"

void stateMenuPlay()
{
  scorePlayer = 0;
  blobNumbers = 0;
  blobFrame = 0;
  initPlayfield();                        // let's clean up the playfield and start with a fresh one
  removeFlag();
  fillBlobPit();                          // fill the pit with random blobs.
  createCurrentBlobs();
  giveExtraScore = false;
  canMoveBlobsDown = true;
  showCombo = false;
  showSpeedUp = false;
  showNod = false;
  showPop = false;
  showChain = false;
  currentSpeed = SPEED_STATE_START;
  previousSpeed = currentSpeed;
  chain = 0;
  elfState = ELF_NORMAL;
  elfStressedFrame = 0;
  elfPausedFrame = 0;
  thumbsUpFrame = 0;
  nodFrame = 0;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  if (arduboy.everyXFrames(currentSpeed) && (!showSpeedUp))dropBlobs();
  updateStage();
  testSpeed();
  if (arduboy.justPressed(RIGHT_BUTTON)) moveBlobsRight();
  else if (arduboy.justPressed(LEFT_BUTTON)) moveBlobsLeft();
  else if (arduboy.justPressed(DOWN_BUTTON)) dropBlobs();
  if (arduboy.justPressed(B_BUTTON)) rotateBlobsRight();
  else if (arduboy.justPressed(A_BUTTON)) rotateBlobsLeft();
  if (arduboy.justPressed(UP_BUTTON))
  {
    gameState = STATE_GAME_PAUSE;
    elfState = ELF_PAUSED;
  }
};

void stateGamePause()
{
  elfState = ELF_PAUSED;
  updateStage();
  if (arduboy.justPressed(UP_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
    elfState = ELF_NORMAL;
  }
};

void stateGameOver()
{
  drawDitherBackground();
  drawStressedElf();
  arduboy.drawRect(0, 0, 51, 64, WHITE);
  
  //sprites.drawSelfMasked(9, 10, you, 0);
  sprites.drawSelfMasked(4, 12, youLose, 0);
  arduboy.fillRect(4, 38, 43, 9, WHITE);
  scoreDraw(6, 40);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
};

#endif
