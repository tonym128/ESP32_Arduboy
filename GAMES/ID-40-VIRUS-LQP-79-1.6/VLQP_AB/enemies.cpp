#include "enemies.h"
#include "player.h"
#include "pickup.h"

// globals ///////////////////////////////////////////////////////////////////

Enemy zombies[ZOMBIE_MAX];


// method implementations ////////////////////////////////////////////////////

// setZombie
// sets the position of a zombie, and enables that instance
void setZombie(Enemy& obj, int x, int y, int16_t type)
{
  obj.frame = 0;
  obj.active = true;
  obj.direction = ENEMY_FACING_WEST;
  obj.x = x;
  obj.y = y;
  obj.type = type;
  if (obj.type) obj.health = 2;
  else obj.health = 3;
  obj.flashTime = 0;
}

// spawnZombie
// adds a zombie in a random place in the map
// returns true if success, false if failure
bool spawnZombie()
{
  int x = random(16, LEVEL_WIDTH - ZOMBIE_WIDTH - 16);
  int y = random(16, LEVEL_HEIGHT - ZOMBIE_HEIGHT - 16);

  if ((x < coolGirl.x - WIDTH) || (x > coolGirl.x + WIDTH) || (y < coolGirl.y - HEIGHT) || (y > coolGirl.y + HEIGHT))
  {
    return addZombie(x, y);
  }

  return false;
}


// addZombie
// searches the zombies list for an empty slot, adds one if available
// returns true if successful, false otherwise
bool addZombie(int x, int y)
{
  int16_t id;

  for (id = 0; id < ZOMBIE_MAX; id++)
  {
    if (!zombies[id].active)
    {
      setZombie(zombies[id], x, y, random(3));
      return true;
      break;
    }
  }

  return false;
}


// updateZombie
// updates the zombie according to game rules
// zombies are "removed" (set inactive) when health reaches zero
void updateZombie(Enemy& obj)
{
  int16_t vx = 0;
  int16_t vy = 0;

  if (arduboy.everyXFrames(ZOMBIE_STEP_DELAY))
  {
    ///////////
    // input //
    ///////////

    // chase player
    if (obj.x < coolGirl.x) vx = ZOMBIE_SPEED;
    if (obj.x > coolGirl.x) vx = -ZOMBIE_SPEED;

    if (obj.y < coolGirl.y) vy = ZOMBIE_SPEED;
    if (obj.y > coolGirl.y) vy = -ZOMBIE_SPEED;

    // if out of bounds, delete this
    if ((obj.x < 0) || (obj.y < 0) || (obj.x >= LEVEL_WIDTH) || (obj.y >= LEVEL_HEIGHT))
    {
      obj.active = false;
      return;
    }

    // update orientation
    if (vx < 0)
      obj.direction = ENEMY_FACING_WEST;
    else if (vx > 0)
      obj.direction = ENEMY_FACING_EAST;

    ////////////////////////
    // horizontal physics //
    ////////////////////////

    // update position
    obj.x += vx;

    // collide with other zombies
    zombieCollide(obj.x, obj.y, true, vx, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);

    // collide with player
    if (zombieCollision(obj, coolGirl.x, coolGirl.y, PLAYER_WIDTH, PLAYER_HEIGHT))
    {
      if (vx > 0)
        obj.x = coolGirl.x - ZOMBIE_WIDTH;
      else if (vx < 0)
        obj.x = coolGirl.x + PLAYER_WIDTH;

      playerHealthOffset(coolGirl, -1);
      vx = 0;
    }

    // collide with walls
    mapCollide(obj.x, obj.y, true, vx, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);

    //////////////////////
    // vertical physics //
    //////////////////////

    // update position
    obj.y += vy;

    // collide with other zombies
    zombieCollide(obj.x, obj.y, false, vy, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);

    // collide with player
    if (zombieCollision(obj, coolGirl.x, coolGirl.y, PLAYER_WIDTH, PLAYER_HEIGHT))
    {
      if (vy > 0)
        obj.y = coolGirl.y - ZOMBIE_HEIGHT;
      else if (vy < 0)
        obj.y = coolGirl.y + PLAYER_HEIGHT;

      playerHealthOffset(coolGirl, -1);
      vy = 0;
    }

    // collide with walls
    mapCollide(obj.x, obj.y, false, vy, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);

    ///////////////
    // animation //
    ///////////////

    if (vx || vy)
    {
      // Advance animation frame
      if (arduboy.everyXFrames(ZOMBIE_FRAME_SKIP)) obj.frame++;

      // Just 4 frames
      if (obj.frame >= ZOMBIE_FRAME_COUNT ) obj.frame = 0;
    }
    else
    {
      obj.frame = 0;
    }
  }

  if (obj.health == 0)
  {
    obj.active = false;
  }
}


// updateZombies
// updates every active zombie in the list
void updateZombies()
{
  int16_t i;

  for (i = 0; i < ZOMBIE_MAX; i++)
  {
    if (!zombies[i].active) continue;
    updateZombie(zombies[i]);
  }
}


// drawZombie
// draws a single zombie
void drawZombie(Enemy& obj)
{
  short drawX, drawY;
  if (obj.active)
  {
    if (obj.flashTime > 0)
    {
      obj.flashTime--;
    }
    if ((obj.flashTime % 4) < 2)
    {
      drawX = obj.x - mapPositionX;
      drawY = obj.y - mapPositionY;
      if (obj.type)
      {
        sprites.drawErase(drawX, drawY, enemyZombieMask, obj.direction);
        sprites.drawSelfMasked(drawX, drawY, enemyZombie, obj.frame + 8 * obj.direction);
      }
      else
      {
        sprites.drawErase(drawX, drawY, zombieTwoMask, obj.direction);
        sprites.drawSelfMasked(drawX, drawY, zombieTwo, (obj.frame%2) + 2 * obj.direction);
      }
    }
  }
  else if (obj.flashTime > 0)
  {
    obj.flashTime--;
    arduboy.drawCircle(obj.x - mapPositionX + ZOMBIE_WIDTH / 2, obj.y - mapPositionY + ZOMBIE_HEIGHT / 2, 12 - obj.flashTime * 2, 1);
  }
}


// drawZombies
// draws every active zombie in the zombie list
void drawZombies()
{
  int16_t id;

  // Draw all the zombies!
  for (id = 0; id < ZOMBIE_MAX; id++)
  {
    drawZombie(zombies[id]);
  }
}


// zombieHealthOffset
// takes a value to be added to zombie health
// kills the zombie if health goes below zero
bool zombieHealthOffset(Enemy& obj, int8_t amount)
{
  obj.health += amount;

  // killed
  if (obj.health <= 0)
  {
    sound.tone(220, 20);
    obj.flashTime = ZOMBIE_FLASH_TIME;
    obj.active = false;
    rollingScore += 100;
    addPickup(obj.x + 4, obj.y + 4);
  }
  else if (amount < 0)
  {
    obj.flashTime = ZOMBIE_FLASH_TIME;
    sound.tone(640, 20);
  }
}


// zombieCollision
// takes zombie id, collision box to test against
// returns true if collision boxes intersect
bool zombieCollision(Enemy& obj, int x, int y, int8_t w, int8_t h)
{
  return
    ( obj.active ) &&
    ( obj.x < x + w ) &&
    ( obj.x + ZOMBIE_WIDTH > x ) &&
    ( obj.y < y + h ) &&
    ( obj.y + ZOMBIE_HEIGHT > y );
}

// clearZombies
// clears the entire list of zombies
void clearZombies()
{
  int16_t id;

  for (id = 0; id < ZOMBIE_MAX; id++)
  {
    zombies[id].active = false;
  }
}

void zombieCollide(int &x, int &y, bool horizontal, int16_t &vel, int8_t w, int8_t h)
{
  int16_t id;
  for (id = 0; id < ZOMBIE_MAX; id++)
  {
    if (zombieCollision(zombies[id], x, y, w, h))
    {
      if (x == zombies[id].x && y == zombies[id].y) continue;
      if (horizontal)
      {
        if (vel > 0)
          x = zombies[id].x - w;
        else if (vel < 0)
          x = zombies[id].x + ZOMBIE_WIDTH;
      }
      else
      {
        if (vel > 0)
          y = zombies[id].y - h;
        else if (vel < 0)
          y = zombies[id].y + ZOMBIE_HEIGHT;
      }
      vel = 0;
    }
  }
}
