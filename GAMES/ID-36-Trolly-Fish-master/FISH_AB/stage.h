#ifndef STAGE_H
#define STAGE_H

#include <Arduino.h>
#include "globals.h"

#define maxVal_ENEMIES                9
#define SPAWN_DELAY                160
#define SCORE_SMALL_FONT           0
#define SCORE_BIG_FONT             1

#define PU_SHOOTFISH               0
#define PU_TURNFISH                1
#define PU_MAGNETFISH              2
#define PU_POPFISH                 3
#define PU_PROTECTFISH             4
#define PU_LIFEFISH                5
#define PU_SHOCKFISH               6
#define PU_STOPFISH                7

#define PU_ON                      1
#define PU_OFF                     0

#define LANEOFFSET                 4
#define JELLYSPAWN                 400
#define JELLYSPAWN2                800
#define JELLYSPAWN3                1200
#define EELSPAWN                   200
#define EELSPAWN2                  400
#define EELSPAWN3                  1000
#define FASTSPAWN                  500
#define FASTSPAWN2                 1000

#define LANE_SEP                   25

extern unsigned long scorePlayer;
extern byte eelmaxVal;
extern byte jellymaxVal;

extern void createPowerUp(byte type);

extern byte pu_shocks;
extern byte pu_bubbles;

extern byte pu_current;

int spawnTimer = 20;
int8_t laneRide = 0;
int fr = 60;
byte seaWeedFrames = 0;
byte seaWeedFrames2 = 3;


struct SmallBubbles
{
  public:
    byte x;
    int y;
    byte speed;
    byte type;
    void resetPos(boolean test);
};

void SmallBubbles::resetPos(boolean test)
{
  x = random(0, 120 - (112*test));
  y = random(0, 64);
  speed = random(1, 4);
  type = random(0, 3);
}

SmallBubbles titleBubbles[16];

void drawBubbles(boolean inGame)
{
  for (byte i = 0; i < 16 -(10*inGame); i++)
  {
    if (arduboy.everyXFrames(3))titleBubbles[i].y = titleBubbles[i].y - titleBubbles[i].speed;
    if (titleBubbles[i].y < -8)titleBubbles[i].resetPos(inGame);
    sprites.drawPlusMask(titleBubbles[i].x, titleBubbles[i].y, smallBubbles_plus_mask, titleBubbles[i].type);
  }
}


void spawnWave()
{
  if (bitRead(powerups,PU_STOPFISH) == PU_OFF)
    spawnTimer--;

  if (spawnTimer <= 0)
  {
    spawnTimer = SPAWN_DELAY + (180 / maxVal(scorePlayer >> 8, 1));

    // Powerup spawns
    if (random(2) == 0)
    {
      createPowerUp(pu_current);
      pu_current = (++pu_current) % 8;
    }

    if (scorePlayer > 1500 && fr == 60)
    {
      fr = 70;
      arduboy.setFrameRate(70);
    }
    if (scorePlayer > 3000 && fr == 70)
    {
      fr = 80;
      arduboy.setFrameRate(80);
    }

    if (scorePlayer > JELLYSPAWN2)
      jellymaxVal = 10;
    if (scorePlayer > EELSPAWN2)
      eelmaxVal = 3;
    if (scorePlayer > FASTSPAWN)
      createEnemy(ENEMY_FAST, (random(3) * LANE_SEP) + LANEOFFSET); // Fillers, tighten gap, faster moving
    if (scorePlayer > EELSPAWN)
      createEnemy(ENEMY_EEL, (random(3) * LANE_SEP) + LANEOFFSET); // Three possible eel lanes, not distruptor, just limits v movement
    if (scorePlayer > EELSPAWN3)
      createEnemy(ENEMY_EEL, 28 + LANEOFFSET); // Three possible eel lanes, not distruptor, just limits v movement
    if (scorePlayer > JELLYSPAWN)
    {
      byte pos = 0;
      if (trollyFish.y < 32)
        --laneRide;
      else
      {
        ++laneRide;
        pos = 1;
      }

      if (abs(laneRide) < 2)
        pos = random(2);
      else
        laneRide = 0;
      createEnemy(ENEMY_JELLY, (pos * 63) + LANEOFFSET); // Two possible jelly lanes, disruptors
    }

    if (scorePlayer > 2000)
      createEnemy(ENEMY_FAST, (random(3) * LANE_SEP) + LANEOFFSET); // Fillers, tighten gap, faster moving

    // There is always enough room between bad fish, jellyfish and eels are what forces a move
    if (scorePlayer < FASTSPAWN2)
      createEnemy(ENEMY_BAD, (random(3) * LANE_SEP) + LANEOFFSET); // Fish are fillers
    else
      createEnemy(ENEMY_FAST, (random(3) * LANE_SEP) + LANEOFFSET); // Fish are fillers
    if (scorePlayer > 50)
      createEnemy(ENEMY_BAD, (random(3) * LANE_SEP) + LANEOFFSET); // Extra fillers
  }
}

boolean checkGameOver()
{
  if (bitRead(powerups,PU_PROTECTFISH)) // protected
    return false;

  Rect player = {.x = 20, .y = trollyFish.y, .width = trollyFish.width, .height = trollyFish.height};
  Rect enemy;
  for (byte i = 0; i < maxVal_ENEMIES; i++)
  {
    enemy.x = enemyFish[i].x;
    enemy.y = enemyFish[i].y;
    enemy.width = enemyFish[i].width;
    enemy.height = enemyFish[i].height;

    // Don't check collisions if enemy is a bubble or dead
    if (enemyFish[i].type != ENEMY_BUBBLE && enemyFish[i].type != ENEMY_DEAD)
    {
      if (bubbleBullet.active)
      {
        Rect bubble = { .x = bubbleBullet.x, .y = bubbleBullet.y, .width = bubbleBullet.width, bubbleBullet.height };
        if (arduboy.collide(enemy, bubble))
        {
          switch (enemyFish[i].type)
          {
            case ENEMY_JELLY:
              numJellys--;
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 3);
              break;
            case ENEMY_EEL:
              numEels--;
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 5);
              break;
            case ENEMY_FAST:
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 2);
              break;
            default:
              giveBonus(enemyFish[i].x, enemyFish[i].y + 8, 1);
          }
          sound.tone(200, 50);
          enemyFish[i].type = ENEMY_BUBBLE;
            
          bubbleBullet.active = false;
        }
      }
        
      if (arduboy.collide(enemy, player))
      {
        
        if (bitRead(powerups,PU_LIFEFISH)) // extra life
        {
          sound.tone(280, 50);
          bitClear(powerups,PU_LIFEFISH);
          enemyFish[i].x -= 32;
          enemyFish[i].resetPos();
          trollyFish.blink = 36;
          return false;
        }
        arduboy.setFrameRate(60);
        fr = 60;
        for (byte note = 0 ; note < 30; note++)
        {
          sound.tone(400 - (note * 10), 300);
          delay(15);
        }
        return true;
      }
    }
  }
  return false;
};



void drawScore(byte scoreX, byte scoreY, byte fontType)
{
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  ltoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 6 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawPlusMask(scoreX + (7 * i), scoreY, numbersSmall_plus_mask, 0);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (10 * i), scoreY, numbersBig, 0);
        break;
    }
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawPlusMask(scoreX + (pad * 7) + (7 * i), scoreY, numbersSmall_plus_mask, digit);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (pad * 10) + (10 * i), scoreY, numbersBig, digit);
        break;
    }
  }
}

void drawHUDPowerUps()
{
  byte offset = 0;

  if (bitRead(powerups,PU_STOPFISH))
  {
    sprites.drawPlusMask(1, 1, hudAssets_plus_mask, 0);
    offset += 8;
  }

  if (bitRead(powerups,PU_SHOOTFISH) && pu_bubbles > 0)
  {
    sprites.drawPlusMask(1 + offset, 1, hudAssets_plus_mask, 1);
    offset += 8;
    sprites.drawPlusMask(1 + offset, 1, numbersSmall_plus_mask, pu_bubbles);
    offset += 8;
  }

  if (bitRead(powerups,PU_PROTECTFISH))
  {
    sprites.drawPlusMask(1 + offset, 1, hudAssets_plus_mask, 2);
    offset += 8;
  }

  if (bitRead(powerups,PU_LIFEFISH))
  {
    sprites.drawPlusMask(1 + offset, 1, hudAssets_plus_mask, 3);
    offset += 8;
  }

  if (bitRead(powerups,PU_SHOCKFISH) && pu_shocks > 0)
  {
    sprites.drawPlusMask(1 + offset, 1, hudAssets_plus_mask, 4);
    offset += 8;
    sprites.drawPlusMask(1 + offset, 1, numbersSmall_plus_mask, pu_shocks);
    offset += 8;
  }

  if (bitRead(powerups,PU_MAGNETFISH))
  {
    sprites.drawPlusMask(1 + offset, 1, hudAssets_plus_mask, 5);
    offset += 8;
  }
}

void drawWeed()
{
  if (arduboy.everyXFrames(8))
  {
    seaWeedFrames++;
    seaWeedFrames2++;
  }
  if (seaWeedFrames > 7 ) seaWeedFrames = 0;
  if (seaWeedFrames2 > 7 ) seaWeedFrames2 = 0;
  for (byte i = 1; i < 15; i++)
  {
    sprites.drawSelfMasked(8 * i, 56, seaWeedSmall, seaWeedFrames);
  }
  sprites.drawSelfMasked(0, 8, seaWeedBig, seaWeedFrames);
  sprites.drawSelfMasked(5, 8, seaWeedBig, seaWeedFrames2);
  sprites.drawSelfMasked(115, 8, seaWeedBig, seaWeedFrames);
  sprites.drawSelfMasked(120, 8, seaWeedBig, seaWeedFrames2);
}

struct Bonus
{
  int8_t x, y;
  byte amount;
  bool active;
};

Bonus bonus[maxVal_ENEMIES];
byte numBonus = 0;

void initBonuses()
{
  for (byte i = 0; i < maxVal_ENEMIES; ++i)
  {
    bonus[i].x = 0;
    bonus[i].y = 0;
    bonus[i].amount = 0;
    bonus[i].active = false;
  }
}

void giveBonus(int8_t x, int8_t y, byte amount)
{
  scorePlayer += amount * 10;
  for (byte i = 0; i < maxVal_ENEMIES; ++i)
  {
    if (bonus[i].active == false)
    {
      bonus[i].active = true;
      bonus[i].x = x;
      bonus[i].y = y;
      bonus[i].amount = amount;
      ++numBonus;

      break;
    }
  }
}

void updateBonus()
{
  if (numBonus > 0 && arduboy.everyXFrames(2))
  {
    for (byte i = 0; i < maxVal_ENEMIES; ++i)
    {
      if (bonus[i].active)
      {
        --bonus[i].y;

        sprites.drawPlusMask(bonus[i].x, bonus[i].y, bonusPoints_plus_mask, bonus[i].amount - 1);

        if (bonus[i].y < 0)
        {
          bonus[i].active = false;
          --numBonus;
        }
      }
    }
  }
}

#endif
