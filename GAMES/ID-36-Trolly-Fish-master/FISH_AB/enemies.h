#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"

#define maxVal_ENEMIES                9
#define GAME_LEFT                  3
#define GAME_TOP                   3
#define GAME_BOTTOM                58

#define ENEMY_BAD                  0
#define ENEMY_JELLY                1
#define ENEMY_EEL                  2
#define ENEMY_STAR                 3 // enemy turned into starfish
#define ENEMY_BUBBLE               4 // enemy turned into bubbles
#define ENEMY_FAST                 5 // fast fish looks like bad fish but is faster
#define ENEMY_DEAD                 6 // shocked fish floats to top

#define BURST_LENGTH               15
#define BURST_WAIT                 20

// Powerups
#define PU_SHOOTFISH               0
#define PU_TURNFISH                1
#define PU_MAGNETFISH              2
#define PU_POPFISH                 3
#define PU_PROTECTFISH             4
#define PU_LIFEFISH                5
#define PU_SHOCKFISH               6
#define PU_STOPFISH                7

#define PUT_STOP                   0

extern const unsigned char starFish_plus_mask[];
extern byte pu_timers[];
extern unsigned long scorePlayer;
extern void giveBonus(int8_t, int8_t, byte);

byte fishFrame = 0;

// Enemy Type variables
int8_t numEels = 0;
int8_t numJellys = 0;
byte jellymaxVal = 1;
byte eelmaxVal = 1;

struct Enemies
{
  public:
    int16_t x;
    int16_t y;
    byte width, height;
    int8_t xSpeed, ySpeed;

    int8_t burst;
    int8_t burstTimer;

    byte type;
    bool active;

    void resetPos();
};

void Enemies::resetPos()
{
  x = 144  + random(12);


  if (type == ENEMY_JELLY)
    --numJellys;

  if (type == ENEMY_EEL)
    --numEels;

  active = false;
}

Enemies enemyFish[maxVal_ENEMIES];

void createEnemy(byte type, byte y)
{
  if (type == ENEMY_JELLY && numJellys >= jellymaxVal)
    return;
  if (type == ENEMY_EEL && numEels >= eelmaxVal)
    return;

  for (byte i = 0; i < maxVal_ENEMIES; i++)
  {
    if (enemyFish[i].active == false)
    {
      enemyFish[i].active = true;
      enemyFish[i].type = type;
      enemyFish[i].xSpeed = -4;
      enemyFish[i].ySpeed = 0;
      enemyFish[i].width = 16;
      enemyFish[i].height = 14;
      enemyFish[i].y = y;

      if (type == ENEMY_FAST)
      {
        enemyFish[i].xSpeed = -3;
        enemyFish[i].y += random(-2, 2);
      }
      if (type == ENEMY_JELLY)
      {
        enemyFish[i].ySpeed = -2;
        enemyFish[i].height = 20;
        enemyFish[i].xSpeed = -3;
        numJellys++;
      }
      if (type == ENEMY_EEL)
      {
        enemyFish[i].xSpeed = -2;
        enemyFish[i].width = 76;
        enemyFish[i].height = 10;
        enemyFish[i].x = 144;
        numEels++;
      }

      return;
    }
  }
}

void updateEnemies()
{
  if (bitRead(powerups,PU_STOPFISH))  // enemies paused
    return;

  for (byte i = 0; i < maxVal_ENEMIES; i++)
  {
    if (enemyFish[i].active)
    {
      switch (enemyFish[i].type)
      {
        case ENEMY_BAD:
          // ----- Bad Fishy -----
          // Bursts forward, with pause
          if (enemyFish[i].burst > 0)
          {
            // Move while bursting
            enemyFish[i].x +=  enemyFish[i].xSpeed;
            --enemyFish[i].burst;
          }
          else
          {
            // Decrement time before next burst
            --enemyFish[i].burstTimer;
            if (enemyFish[i].burstTimer == 0)
            {
              // Timer up, reset burst and burstTimer
              enemyFish[i].burstTimer = BURST_WAIT;
              enemyFish[i].burst = BURST_LENGTH;
            }
          }
          // Shock fish to death if effect is on and within range
          if (shock_burst > 0)/*(getPowerup(PU_SHOCKFISH) && abs(enemyFish[i].x - trollyFish.x) < 32
              && abs(enemyFish[i].y - trollyFish.y) < 32)*/
          {
            enemyFish[i].type = ENEMY_DEAD;
            giveBonus(enemyFish[i].x, enemyFish[i].y, 1);
          }
          break;

        case ENEMY_FAST:
          // ----- Fast Fishy -----
          // Bursts forward, with pause
          if (enemyFish[i].burst > 0)
          {
            // Move while bursting
            enemyFish[i].x +=  enemyFish[i].xSpeed;
            --enemyFish[i].burst;
          }
          else
          {
            // Decrement time before next burst
            --enemyFish[i].burstTimer;
            if (enemyFish[i].burstTimer == 0)
            {
              // Timer up, reset burst and burstTimer
              enemyFish[i].burstTimer = BURST_WAIT >> 1;
              enemyFish[i].burst = (BURST_LENGTH << 1) - 3;
            }
          }
          // Shock fish to death if effect is on and within range
          if (shock_burst > 0)
          {
            enemyFish[i].type = ENEMY_DEAD;
            giveBonus(enemyFish[i].x, enemyFish[i].y, 2);
          }
          break;

        case ENEMY_JELLY:
          // ----- Jelly Fish -----
          // Bursts upward
          // Moves faster horizontally when bursting
          if (enemyFish[i].burst > 0)
          {
            // Move up while bursting
            enemyFish[i].y +=  enemyFish[i].ySpeed;
            // Faster left movement when bursting
            enemyFish[i].x +=  enemyFish[i].xSpeed;

            if (enemyFish[i].y <= -4)
              enemyFish[i].burst = 1;

            --enemyFish[i].burst;
            if (enemyFish[i].burst <= 0)
            {
              enemyFish[i].burst = 0;
              enemyFish[i].burstTimer = BURST_WAIT;
            }
          }
          else
          {
            // Slower left movement not bursting
            enemyFish[i].x--;

            // Decrement time before next burst
            --enemyFish[i].burstTimer;

            // Drop y
            enemyFish[i].y -= enemyFish[i].ySpeed / 2;
            if (enemyFish[i].y > GAME_BOTTOM - (enemyFish[i].height / 2))
              enemyFish[i].burstTimer = 0;            // Reset timer when under screen

            if (enemyFish[i].burstTimer <= 0)
            {
              // Timer up, reset burst and burstTimer
              enemyFish[i].burst = (BURST_LENGTH / 2) + 2;
              enemyFish[i].burstTimer = 0;
            }
          }
          break;

        case ENEMY_EEL:
          // ----- Eel -----
          // Steady left movement
          enemyFish[i].x +=  enemyFish[i].xSpeed;

          break;

        case ENEMY_DEAD:
          // ----- Dead Fish -----
          // Steady up movement
          enemyFish[i].y--;
          if (enemyFish[i].y < -enemyFish[i].height)
          {
            enemyFish[i].resetPos();
            scorePlayer++;
          }

          break;

        case ENEMY_STAR:
          if (bitRead(powerups,PU_MAGNETFISH))
          {
            {
              if (enemyFish[i].y < trollyFish.y) enemyFish[i].y++;
              if (enemyFish[i].y > trollyFish.y) enemyFish[i].y--;
            }
          }

          enemyFish[i].x -= 4;
          break;

        case ENEMY_BUBBLE:
          enemyFish[i].y--;
          if (enemyFish[i].y < 1) enemyFish[i].resetPos();
          break;
      }

      // Outside of room, deactivate
      if ( enemyFish[i].x < (GAME_LEFT - enemyFish[i].width))  enemyFish[i].resetPos();
    }
  }
}

void drawEnemies()
{
  if (arduboy.everyXFrames(6)) fishFrame++;
  if (fishFrame > 3 || bitRead(powerups,PU_STOPFISH)) fishFrame = 0;
  if (pu_timers[PUT_STOP] > 40 || pu_timers[PUT_STOP] % 8 < 6)
  {
    for (byte i = 0; i < maxVal_ENEMIES; i++)
    {
      switch (enemyFish[i].type)
      {
        case ENEMY_BAD:
          sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 1, badFishy_plus_mask, (fishFrame * (minVal(enemyFish[i].burst, 1))));
          break;

        case ENEMY_FAST:
          sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 1, badFishy_plus_mask, (fishFrame * (minVal(enemyFish[i].burst, 1))));
          break;

        case ENEMY_JELLY:
          {
            byte jf = (enemyFish[i].burst <= 0) ? minVal((((enemyFish[i].burstTimer * 10) / BURST_WAIT * 3) / 10) + 1, 3) : 0;
            sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 4, jellyFish_plus_mask, jf);
          }
          break;

        case ENEMY_EEL:
          sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 3, eel_plus_mask, fishFrame);
          break;

        case ENEMY_STAR:
          sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 1, starFish_plus_mask, 0);
          break;

        case ENEMY_BUBBLE:
          sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y, bubbles_plus_mask, enemyFish[i].y % 13);
          break;

        case ENEMY_DEAD:
          {
            byte df = (shock_burst > 0) ? (shock_burst % 2) + 4 : 6;
            sprites.drawPlusMask(enemyFish[i].x, enemyFish[i].y - 1, badFishy_plus_mask, df);
          }
          break;
      }
    }
  }
}


#endif
