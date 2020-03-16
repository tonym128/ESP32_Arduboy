#include <avr/pgmspace.h>
#include <Gamby.h>
#include "runner.h"
#include "stage.h"
#include "stage_bitmap.h"
#include "item.h"
#include "scenario.h"
#include "sound.h"

#define PAUSE_BETWEEN_SOUNDS 500

extern GambyGraphicsMode gamby;
extern unsigned char level;
extern int life;
extern unsigned long score;
extern unsigned char countlevels;
extern int itemX[];
extern int itemY[];
extern int runnerX;
extern int runnerY;
extern boolean soundyesno;


byte showitems =  B00000000;        // this byte holds all the items the player runs into during the game
//                 ||||||||
//                 |||||||└->  stone1        
//                 ||||||└-->  stone2 
//                 |||||└--->  bird1
//                 ||||└---->  bird2
//                 |||└----->  bird3
//                 ||└------>  extra life
//                 |└------->  not used
//                 └-------->  not used

char birdframe = 0;
boolean heartframe = 0;
int background1step = 96;
int background2step = 0;
int ground1step = 96;
int ground2step = 0;
int foregroundstep = 80;
byte foregroundid =0;
byte background1id = 0;
byte background2id = 1;
byte groundid = 0;

unsigned long next_life = 0;
unsigned long next_sound = 0;
char stage = 0;
char scenario = 0;

boolean collide(int x1, int y1, int x2, int y2, int xx1, int yy1, int xx2, int yy2)
{
     if (x2 <= xx1) return false;
     else if (x1 >= xx2) return false;
     else if (y2 <= yy1) return false;
     else if (y1 >= yy2) return false;
     else return true;
}

void stage_update()
{
  gamby.rect(0,52,95,60);

  background_draw();
  ground_draw();
  items_draw();
  runner_draw();
  forground_draw();

  items_update();

  items_collision();

  //draw score and lives on the screen
  life_draw();
  score_draw(28,52);
  level_update();

  gamby.update();
  score++;
}


void background_draw()
{
  if (background1step == 96) background1id = random(0,5);
  if (background2step == 96) background2id = random(0,3);
  gamby.drawSprite(background1step, -4, background_bitmap,background1id);
  gamby.drawSprite(background2step, -4, background_bitmap,background2id);
  background1step -= 2;
  background2step -= 2;
  if (background1step < -95) background1step = 96;
  if (background2step < -95) background2step = 96;
}

void forground_draw()
{
  if (foregroundstep == 80) foregroundid = random(0,3);
  gamby.drawSprite(foregroundstep, -4, foreground_bitmap,foregroundid,foreground_bitmap,foregroundid);
  foregroundstep -= 8;
  if (foregroundstep < -100) foregroundstep = 80;
}

void ground_draw()
{
  if (ground1step == 96) groundid = random(0,5);
  gamby.drawSprite(ground1step, 36, ground_bitmap,groundid, ground_bitmap,groundid);
  gamby.drawSprite(ground2step, 36, ground_bitmap,ground_bitmap);
  ground1step -= 6;
  ground2step -= 6;
  if (ground1step < -95) ground1step = 96;
  if (ground2step < -95) ground2step = 96;
}

void life_draw()
{
  if (millis() >= next_life)
  {
    life--;
    if (countlevels > 0) life -=2;
    if (countlevels > 1) life -=2;
    if (countlevels > 2) life -=2;
    if (countlevels > 3) life -=2;
    if (countlevels > 4) life -=2;
    
    if (life <48)
    {
      switch (level)
      {
        case 1: case 2: case 3:
          scenario = 10;
          break;
        case 4: case 5: case 6:
          scenario = 11;
          break;
        case 7: case 8: case 9:
          scenario = 12;
          break;
        case 10:
          scenario = 13;
          break;   
      }
    }
    //if (life < 2) life = 96; // just for testing remove later !!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (countlevels < 1) next_life = millis() + (550 - (50*level)) ;
  }
  gamby.drawSprite(0, 52, life_bitmap);
  gamby.drawPattern = PATTERN_WHITE;
  gamby.rect(0,61,96,63);
  gamby.drawPattern = PATTERN_BLACK;
  gamby.rect(0, 62, life, 63); 
}

void score_draw(int scoreX, int scoreY)
{
  gamby.drawSprite(scoreX, scoreY, score_bitmap);
  char buf[8];
  itoa(score, buf, 8);
  char charLen = strlen(buf);
  char pad = 8-charLen;

  //draw 0 padding
  for (byte i=0; i< pad; i++)
  {
    gamby.drawSprite(scoreX+28+(5*i), scoreY, numbers_bitmap);
  }

  for (byte i=0; i< charLen; i++)
  {
    char digit = buf[i];
    if (digit <= 48)
    {
      digit = 0;
    } 
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    gamby.drawSprite(scoreX+28+(pad*5)+(5*i), scoreY, numbers_bitmap, digit);
  }
}

void items_draw()
{
  stone1_draw();
  stone2_draw();
  bird1_draw();
  bird2_draw();
  bird3_draw();
  heart_draw();
  birdframe++;
  if (birdframe > 7) birdframe = 0;
  heartframe = !heartframe;
}

void items_update()
{
  switch (scenario)
  {
  case 0:
    switch (level)
    {
      case 1:
        scenario = random (1,4);
        break;
      case 2:
        scenario = random (1,5);
        break;
      case 3:
        scenario = random (1,6);
        break;
      case 4:
        scenario = random (1,7);
        break;
      case 5:
        scenario = random (2,8);
        break;
      case 6:
        scenario = random (3,9);
        break;
      case 7:
        scenario = random (4,9);
        break;
      case 8:
        scenario = random (5,9);
        break;
      case 9:
        scenario = random (6,10);
        break;
      case 10:
        scenario = random (7,10);
        break;
    } 
    break;
  case 1:
    scenario1();
    break;
  case 2:
    scenario2();
    break;
  case 3:
    scenario3();
    break;
  case 4:
    scenario4();
    break;
  case 5:
    scenario5();
    break;
  case 6:
    scenario6();
    break;
  case 7:
    scenario7();
    break;
  case 8:
    scenario8();
    break;
  case 9:
    scenario9();
    break;
    
  case 10:
    scenario1EL();
    break;
  case 11:
    scenario2EL();
    break;
  case 12:
    scenario3EL();
    break;
  case 13:
    scenario4EL();
    break;
  }
}

void items_collision()
{
  //check item stone 1
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[0]+3,itemY[0]+2,itemX[0]+13,itemY[0]+16)== true)
  {
    life -= 5;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_hit_stone();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
  
  //check item stone 2
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[1]+3,itemY[1]+2,itemX[1]+13,itemY[1]+16)== true)
  {
    life -= 5;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_hit_stone();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
  
  //check item bird 1
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[2]+2,itemY[2],itemX[2]+16,itemY[2]+16)== true)
  {
    life -= 5;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_hit_bird();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
  
  //check item bird 2
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[3]+2,itemY[3],itemX[3]+16,itemY[3]+16)== true)
  {
    life -= 5;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_hit_bird();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
  
  //check item bird 3
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[4]+2,itemY[4],itemX[4]+16,itemY[4]+16)== true)
  {
    life -= 5;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_hit_bird();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
  
  //check item hart
  if (collide(runnerX+7,runnerY,runnerX+19,runnerY+24,itemX[5]+2,itemY[5],itemX[5]+12,itemY[5]+16)== true)
  {
    life = 96;
    if (millis() >= next_sound && soundyesno == true)
    {
      sound_extra_life();
      next_sound = millis() + PAUSE_BETWEEN_SOUNDS;
    }
  }
}

void level_update()
{
  unsigned long nextlevelscore; 
  if (score > nextlevelscore + 500)
  {
    level++;
    if (level > 10) level = 1;
    nextlevelscore = score;
  }
  if (level > 10) 
  {
    level = 1;
    countlevels++;
  }
}



