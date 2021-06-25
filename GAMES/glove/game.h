#ifndef _GAME_H
#define _GAME_H

#include "data.h"

void displayGame();
void saveGame(uint8_t file);

void gameGotoLevel(uint8_t level);
void doGameComplete();
void doGameOver();
void flashScreen();
void addScore(int16_t points);

void gameSetup();
void gameLoop();

#endif
