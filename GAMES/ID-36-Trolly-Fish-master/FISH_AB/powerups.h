#ifndef POWERUPS_H
#define POWERUPS_H

#include <Arduino.h>
#include "globals.h"

#define MAX_STARS                  8   // Maximum regular stars
#define MAX_ENEMIES                9
#define TOTAL_STARS                MAX_STARS + (MAX_ENEMIES * 3)  // Total initialized stars, including enemies turned
#define TOTAL_TYPES                4
#define STAR_HEIGHT                10

#define GAME_LEFT                  3

// Powerups
#define PU_SHOOTFISH               0
#define PU_TURNFISH                1
#define PU_MAGNETFISH              2
#define PU_POPFISH                 3
#define PU_PROTECTFISH             4
#define PU_LIFEFISH                5
#define PU_SHOCKFISH               6
#define PU_STOPFISH                7

// Powerup Timers
#define PUT_STOP                   0
#define PUT_PROTECT                1
#define PUT_SHOCK                  2
#define PUT_MAGNET                 3

// Powerup Charges
#define PUC_SHOOT                  9
#define PUC_SHOCK                  8

#define MAX_POWERUPS               1
#define PU_ON                      1
#define PU_OFF                     0

extern unsigned long scorePlayer;
extern void giveBonus(int8_t, int8_t, byte);


// Function prototypes
void createStar(byte, byte);

// Total of 8 powerups, 1 byte of flags
byte powerups = 0x00;   //Active powerups
byte pu_timers[4];
byte pu_shocks = 0;
byte pu_bubbles = 0;


struct PowerUp
{
  public:
    uint8_t x;
    uint8_t y;
    byte width, height;
    int8_t xSpeed, ySpeed;
    bool active;
    byte type;
};

PowerUp powerUp = {
  .x = 128, .y = 32, .width = 14,
  .height = 14, .xSpeed = -1, .ySpeed = 0,
  .active = false, .type = 0
};

PowerUp bubbleBullet = {
  .x = 0, .y = 0, .width = 10,
  .height = 10, .xSpeed = 2, .ySpeed = 0,
  .active = false, .type = 0,
};

void shootBubble()
{
  if (!bubbleBullet.active && pu_bubbles > 0)
  {
    bubbleBullet.x = 16;
    bubbleBullet.y = trollyFish.y;
    bubbleBullet.xSpeed = 5;
    bubbleBullet.ySpeed = 0;
    bubbleBullet.active = true;

    --pu_bubbles;

    for (byte note = 0 ; note < 30; note++)
    {
      sound.tone(700 - (note * 10), 1);
      delay(1);
    }
  }
}

void updateBubble()
{
  if (bubbleBullet.active)
  {
    bubbleBullet.x += bubbleBullet.xSpeed;
    bubbleBullet.y += bubbleBullet.ySpeed;

    if (bubbleBullet.y > 64)
    {
      bubbleBullet.active = false;
    }

    if (arduboy.everyXFrames(6) && bubbleBullet.xSpeed > 0)
    {
      --bubbleBullet.xSpeed;
      --bubbleBullet.ySpeed;
    }

    sprites.drawPlusMask(bubbleBullet.x, bubbleBullet.y - 1, bubbleBullet_plus_mask, 0);
  }
}

void createPowerUp(byte type)
{
  if (powerUp.active) // already a powerup spawned
    return;

  // No duplicates on timed or consumed powerups

  powerUp.active = true;
  powerUp.x = 158;
  powerUp.y = random(4, 50);
  powerUp.type = type;
}


// Event when collision with powerup
void triggerPowerUp(byte type)
{
  sound.tone(1300, 150);
  switch (type)
  {
    case PU_SHOOTFISH:
      if (pu_shocks == 0)
      {
        pu_bubbles = PUC_SHOOT;
        bitSet(powerups,PU_SHOOTFISH);
      }
      else {
        giveBonus(40, trollyFish.y, 2);
      }
      break;
    case PU_TURNFISH:
      for (byte i = 0; i < MAX_ENEMIES; ++i)
      {
        if (enemyFish[i].active && enemyFish[i].x < 135)
        {
          if (enemyFish[i].type == ENEMY_JELLY) numJellys--;
          if (enemyFish[i].type == ENEMY_EEL) numEels--;

          createStar(enemyFish[i].x, enemyFish[i].y + 3);
          createStar(enemyFish[i].x + 6, enemyFish[i].y);
          createStar(enemyFish[i].x + 3, enemyFish[i].y + 6);
          enemyFish[i].resetPos();
        }
      }
      break;
    case PU_MAGNETFISH:
      bitSet(powerups,PU_MAGNETFISH);
      pu_timers[PUT_MAGNET] = 255;
      break;
    case PU_POPFISH:
      for (byte i = 0; i < MAX_ENEMIES; ++i)
      {
        if (enemyFish[i].active && enemyFish[i].x < 135)
        {
          switch (enemyFish[i].type)
          {
            case ENEMY_JELLY:
              numJellys--;
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 2);
              break;
            case ENEMY_EEL:
              numEels--;
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 4);
              break;
            default:
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 1);
          }
          enemyFish[i].type = ENEMY_BUBBLE;
        }
      }
      break;
    case PU_PROTECTFISH:
      bitSet(powerups,PU_PROTECTFISH);
      pu_timers[PUT_PROTECT] = 255;
      break;
    case PU_LIFEFISH:
      if (bitRead(powerups,PU_LIFEFISH)) giveBonus(20, trollyFish.y, 5);
      bitSet(powerups,PU_LIFEFISH);
      break;
    case PU_SHOCKFISH:
      if (pu_bubbles == 0)
      {
        bitSet(powerups,PU_SHOCKFISH);
        pu_shocks = PUC_SHOCK;
      }
      else
      {
        giveBonus(40, trollyFish.y, 2);
      }
      break;
    case PU_STOPFISH:
      bitSet(powerups,PU_STOPFISH);
      pu_timers[PUT_STOP] = 153;
      break;
  }
}

void updatePowerUp()
{
  // Decrement timers
  if (arduboy.everyXFrames(3))
  {
    for (byte i = 0; i < 4; ++i)
    {
      if (pu_timers[i] > 0)
      {
        --pu_timers[i];
        if (pu_timers[i] == 0)
        {
          switch (i)
          {
            case PUT_STOP:
              bitClear(powerups,PU_STOPFISH);
              break;
            case PUT_SHOCK:
              bitClear(powerups,PU_SHOCKFISH);
              break;
            case PUT_PROTECT:
              bitClear(powerups,PU_PROTECTFISH);
              break;
            case PUT_MAGNET:
              bitClear(powerups,PU_MAGNETFISH);
              break;
          }
        }
      }
    }
  }

  // Update position, draw, collide
  if (powerUp.active)
  {
    powerUp.x += powerUp.xSpeed;
    if (powerUp.x < GAME_LEFT) powerUp.active = false;
    if (powerUp.x % 24 < 18)
      sprites.drawPlusMask(powerUp.x, powerUp.y - 1, powerUps_plus_mask, powerUp.type);

    Rect playerRect = {.x = 20, .y = trollyFish.y, .width = trollyFish.width, .height = trollyFish.height};

    Rect powerupRect = {.x = powerUp.x, .y = powerUp.y, .width = powerUp.width, powerUp.height};

    if (arduboy.collide(powerupRect, playerRect))
    {
      // Trigger powerup effect
      triggerPowerUp(powerUp.type);
      // Reset powerup
      powerUp.active = false;
      powerUp.x += 128;
    }
  }
}

const byte SIN_Y[] = {
  23, 32, 41, 48,
  54, 48, 41, 32,
};



void initStarFish(byte type);

byte cycles = 3;
byte streak = 0;
byte star_type = 0;

struct GameObject
{
  public:
    uint8_t x;
    uint8_t y;
    byte width, height;
    byte xSpeed, ySpeed;
    bool active;
    byte id;

    void resetPos();

};

GameObject starFish[TOTAL_STARS];

void GameObject::resetPos()
{
  x += 128;

  if (cycles > 0 && id < MAX_STARS)
    --cycles;
  else
    active = false;

  bool done = true;
  for (byte i = 0; i < MAX_STARS; ++i)
  {
    if (starFish[i].active)
      done = false;
  }

  if (done)
  {
    initStarFish(star_type);
    star_type = (++star_type) % TOTAL_TYPES;
  }
}

// Create a bonus star
// Used when enemy is turned into a star
void createStar(byte _x, byte _y)
{
  for (byte i = MAX_STARS; i < TOTAL_STARS; ++i)
  {
    if (starFish[i].active == false)
    {
      starFish[i].x = _x;
      starFish[i].y = _y;
      starFish[i].xSpeed = -2;
      starFish[i].active = true;
      starFish[i].id = i;

      break;
    }
  }
}

void startStarFish()
{
  streak = 0;

  for (byte i = 0; i < TOTAL_STARS; ++i)
  {
    starFish[i].x = 128;
    starFish[i].y = 0;
    starFish[i].width = 8;
    starFish[i].height = STAR_HEIGHT;
    starFish[i].xSpeed = -2;
    starFish[i].ySpeed = 0;
    starFish[i].active = false;
  }
}

void initStarFish(byte type)
{
  switch (type)
  {
    case 0:
      {
        // Big sin wave
        for (byte i = 0; i < MAX_STARS; ++i)
        {
          starFish[i].x = 120 + (128 / MAX_STARS * i);
          starFish[i].y = SIN_Y[i] - 12;
          starFish[i].active = true;
          cycles = 3 * MAX_STARS;
        }
      }
      break;
    case 1:
      {
        byte y = random(5, 48);
        // Small sin wave
        for (byte i = 0; i < MAX_STARS; ++i)
        {
          starFish[i].x = 120 + (128 / MAX_STARS * i);
          starFish[i].y = (SIN_Y[i] / 3) - 12 + y;
          starFish[i].active = true;
          cycles = 3 * MAX_STARS;
        }
      }
      break;
    case 2:
      {
        // Straight line
        byte y = random(10, 54);
        for (byte i = 0; i < MAX_STARS; ++i)
        {
          starFish[i].x = 120 + (128 / MAX_STARS * i);
          starFish[i].y = y;
          starFish[i].active = true;
          cycles = 1 * MAX_STARS;
        }
      }
      break;
    case 3:
      {
        // square cluster
        byte y = random(10, 40);
        for (byte i = 0; i < MAX_STARS; ++i)
        {
          starFish[i].x = 120 + ((i % 4) * 8);
          starFish[i].y = y + ((i / 4) * 8);
          starFish[i].active = true;
          cycles = 0 * MAX_STARS;
        }
      }
      break;
  }
}

void updateStarFish()
{
  for (byte i = 0; i < TOTAL_STARS; ++i)
  {
    if (starFish[i].active)
    {
      if (bitRead(powerups,PU_MAGNETFISH) || i >= MAX_STARS)
      {
        if (arduboy.everyXFrames(2) && abs(20 - starFish[i].x) < 32)
        {
          // Y magnet
          if (starFish[i].y < trollyFish.y) starFish[i].y++;
          if (starFish[i].y > trollyFish.y) starFish[i].y--;

          // X magnet
          if (starFish[i].x < 20) starFish[i].x++;
          if (starFish[i].x > 20) starFish[i].x--;
        }
        if (arduboy.everyXFrames(2) && abs(20 - starFish[i].x) < 42)
        {
          // Y magnet further out
          if (starFish[i].y < trollyFish.y) starFish[i].y++;
          if (starFish[i].y > trollyFish.y) starFish[i].y--;
        }
      }

      starFish[i].x += starFish[i].xSpeed;
      if (starFish[i].x < GAME_LEFT)
      {
        starFish[i].resetPos();
        streak = 0;
      }
      sprites.drawPlusMask(starFish[i].x, starFish[i].y - 1, starFish_plus_mask, 0);
    }
  }
}

void checkIfScored() {
  Rect playerRect = {.x = 20, .y = trollyFish.y, .width = trollyFish.width, .height = trollyFish.height};

  for (byte i = 0; i < TOTAL_STARS; ++i)
  {
    Rect starFishPoint = {.x = starFish[i].x, .y = starFish[i].y, .width = starFish[i].width, starFish[i].height};

    if (arduboy.collide(starFishPoint, playerRect))
    {
      sound.tone(300, 40);
      scorePlayer++;
      starFish[i].resetPos();
      ++streak;
      if (streak >= 8)
      {
        giveBonus(28, trollyFish.y, 1);
        streak = 0;
      }
    }
  }
}

#endif
