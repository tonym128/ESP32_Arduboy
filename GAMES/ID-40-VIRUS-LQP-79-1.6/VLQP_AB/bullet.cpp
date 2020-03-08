#include "bullet.h"
#include "player.h"

// globals ///////////////////////////////////////////////////////////////////
const int16_t BulletXVelocities[8] = {
  0,
  -2,
  -3,
  -2,
  0,
  2,
  3,
  2,
};

Bullet bullets[BULLET_MAX];

// method implementations ////////////////////////////////////////////////////

// setBullet
// sets the position and the velocity of a bullet
bool setBullet(Bullet& obj, int x, int y, uint16_t vx, uint16_t vy)
{
  if (!obj.active)
  {
    obj.x = x;
    obj.y = y;
    obj.vx = vx;
    obj.vy = vy;
    obj.active = true;
    return true;
  }
  return false;
}

// addBullet
// searches the bullet list for an empty slot, adds one if available
void addBullet(int16_t x, int16_t y, int16_t direction, int16_t vx, int16_t vy)
{
  int16_t id;

  for (id = 0; id < BULLET_MAX; id++)
  {
    if (setBullet(bullets[id], x - BULLET_WIDTH / 2, y - BULLET_HEIGHT / 2,
          vx + BulletXVelocities[direction], vy + BulletXVelocities[(direction + 6) % 8]))
    {
      sound.tone(440, 20);
      break;
    }
  }
}


// updateBullet
// updates a bullet according to the game rules
void updateBullet(Bullet& obj)
{
  int16_t id;

  if (obj.active)
  {
    // horizontal physics
    obj.x += obj.vx;

    // vertical physics
    obj.y += obj.vy;

    // collide with zombies
    for (id = 0; id < ZOMBIE_MAX; id++)
    {
      if (zombieCollision(zombies[id], obj.x, obj.y, BULLET_WIDTH, BULLET_HEIGHT))
      {
        obj.active = false;
        zombieHealthOffset(zombies[id], -1);
        break;
      }
    }


    if (getTileType(obj.x / TILE_WIDTH, obj.y / TILE_HEIGHT) > 10)
    {
      obj.active = false;
    }

    // delete if gone off screen
    if ((obj.x < mapPositionX) || (obj.y < mapPositionY) || (obj.x > WIDTH + mapPositionX) || (obj.y > HEIGHT + mapPositionY))
    {
      obj.active = false;
    }
  }
}


// updateBullets
// updates the entire list of bullets
void updateBullets()
{
  int16_t id;

  for (id = 0; id < BULLET_MAX; id++)
  {
    updateBullet(bullets[id]);
  }
}


// drawBullets
// draws the entire list of bullets
void drawBullets()
{
  int16_t id;
  int x, y;

  for (id = 0; id < BULLET_MAX; id++)
  {
    Bullet& bull = bullets[id];

    if (!bull.active) continue;
    x = bull.x - mapPositionX;
    y = bull.y - mapPositionY;
    if ((x > 0) && (y > 0) && (x < WIDTH) && (y < HEIGHT))
    {
      sprites.drawSelfMasked(x, y, dotMask, 0);
    }
  }
}


// clearBullets
// clears the entire list of bullets
void clearBullets()
{
  int16_t id;

  for (id = 0; id < BULLET_MAX; id++)
  {
    bullets[id].active = false;
  }
}
