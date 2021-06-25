#ifndef _SAVE_H
#define _SAVE_H

#include "world.h"
#include "globals.h"

extern bool validSave;

void saveSetup();
void saveCreate();
bool checkFileValidity(uint8_t file);
void breakSave(uint8_t file);
void initializeSave(uint8_t file);
int16_t getRoomClearPercentage();
int16_t getCurrentLevelProgress();
bool saveHighScore();
void markRoomAsCleared(uint8_t room);
void deleteContinueSave();
void displayLoadGame(uint8_t file);
void clearAllRooms();
uint8_t readAdvance(int16_t &address);

#endif
