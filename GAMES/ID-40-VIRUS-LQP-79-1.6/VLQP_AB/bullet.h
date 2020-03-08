#ifndef BULLET_H
#define BULLET_H

#include "enemies.h"
#include "level.h"

// constants /////////////////////////////////////////////////////////////////

#define BULLET_MAX        6
#define BULLET_WIDTH      2
#define BULLET_HEIGHT     2
#define BULLET_DIRECTIONS 8

extern const int16_t BulletXVelocities[8];

// structures ////////////////////////////////////////////////////////////////

struct Bullet {
  int16_t x;
  int16_t y;
  int16_t vx;
  int16_t vy;
  int16_t active;
};

// globals ///////////////////////////////////////////////////////////////////

extern Bullet bullets[BULLET_MAX];

// method prototypes /////////////////////////////////////////////////////////

bool setBullet(Bullet& obj, int16_t x, int16_t y, int16_t vx, int16_t vy);
void addBullet(int16_t x, int16_t y, int16_t direction, int16_t vx, int16_t vy);
void updateBullet(Bullet& obj);
void updateBullets();
void drawBullets();
void clearBullets();

#endif
