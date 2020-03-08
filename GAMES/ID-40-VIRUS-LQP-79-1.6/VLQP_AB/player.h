#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "bitmaps.h"
#include "bullet.h"
#include "elements.h"
#include "enemies.h"

extern int mapPositionX;
extern int mapPositionY;

// constants /////////////////////////////////////////////////////////////////

#define PLAYER_FACING_SOUTH       0
#define PLAYER_FACING_SOUTHWEST   1
#define PLAYER_FACING_WEST        2
#define PLAYER_FACING_NORTHWEST   3
#define PLAYER_FACING_NORTH       4
#define PLAYER_FACING_NORTHEAST   5
#define PLAYER_FACING_EAST        6
#define PLAYER_FACING_SOUTHEAST   7

#define PLAYER_WIDTH              16
#define PLAYER_HEIGHT             16

#define PLAYER_SCREEN_XMIN        52 //(64-12)
#define PLAYER_SCREEN_XMAX        68 //(64+4)
#define PLAYER_SCREEN_YMIN        24 //(32-8)
#define PLAYER_SCREEN_YMAX        24 //(32-8)

#define PLAYER_FLASH_TIME         60
#define PLAYER_MAXHEALTH          8
#define PLAYER_START_HEALTH       5

#define WEAPON_OVERHEAT           35


// structures ////////////////////////////////////////////////////////////////

struct Player
{
  public:
    boolean walking;
    int16_t direction;
    int16_t frame;
    int16_t shotDelay;
    int x;
    int y;
    int16_t health;
    int16_t flashTime;
    int16_t camDirection;
    int16_t diagonalTime;
    boolean coolDownVisible;
    boolean overHeated;
    int16_t coolDownCounter;
};


// globals ///////////////////////////////////////////////////////////////////

extern Player coolGirl;
extern int rollingScore;
extern int16_t pickupsAvailable[10];
extern int16_t pickupsCounter;

// method prototypes /////////////////////////////////////////////////////////

void updatePlayer(Player& obj);
void drawPlayer(Player& obj);
void playerHealthOffset(Player& obj, int8_t amount);
void initializePlayer(Player& obj);
void drawLife(Player& obj);
void drawCoolDown();

#endif
