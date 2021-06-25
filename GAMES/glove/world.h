#ifndef _WORLD_H
#define _WORLD_H

#include "globals.h"
#include "data.h"
#include "game.h"

#include "bullet.h"
#include "badguy.h"
#include "wall.h"
#include "key.h"
#include "treasure.h"
#include "spawner.h"
#include "exit.h"
#include "explorer.h"

const int16_t numBullets = 3;
const int16_t numBadguys = 20;
const int16_t numWalls = 16;
const int16_t numKeys = 4;
const int16_t numTreasures = 9;
const int16_t numSpawners = 4;
const int16_t numExits = 4;


const int16_t gamew = 256;
const int16_t gameh = 128;

extern Bullet bullets[3];
extern BadGuy badguys[20];
extern Explorer p1;
extern Wall walls[16];
extern Key keys[4];
extern Treasure treasures[9];
extern Spawner spawners[4];
extern Exit exits[4];

extern int16_t spawnx;
extern int16_t spawny;
extern int8_t autoFireTime;
extern int8_t whiteScreenTime;
extern uint16_t score;
extern int16_t rollingScore;
extern uint16_t rollingHealth;
extern uint8_t currentLevel;
extern uint8_t levelsCompleted;
extern uint16_t gameTime;
extern int16_t gameTimeTick;
extern bool quitGame;
extern int16_t scrollx;
extern int16_t scrolly;
extern int8_t BadGuy_UpdateDelay;

void worldLoadLevel();
void worldClearLevel();

#endif
